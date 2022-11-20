#include <stdio.h>

#include "../support/logger.h"
#include "../support/shared.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

//Function to replace \n characters
static void printReplaceStr(const char* format, const char* str);

static void genBlocks(Program* program);
static void genStringLinks(String* str);
static void genString(String* str);
static void genConst(Constant* constant);
static void genVal(Value* val);
static void genExpr(Expression* expr);
static void genIfCond(IfCond* ifCond);
static void genWhenThenStatements(WhenThen* whenThen);
static void genMatchCond(MatchCond* matchCond);
static void genConditional(Conditional* cond);
static void genValOrCond(ValOrCond* val);
static void genHeaderTags(Header* header);
static void genHeader(Header* header);
static void genBlock(Block* block);
static void genBlocks(Program* program);
static void genFork(Fork* fork);
static void genTriggerParams(TriggerParameter* params);
static void genText(Text* text);
static void genBodyLinks(Body* body);
static void genBody(Body* body);
static void genBlock(Block* block);
static void genBlocks(Program* program);
static void genFactor(Factor* factor);



//Function to replace \n characters
static void printReplaceStr(const char* format, const char* str) {
	const char* ptr = str;
	
	int enterCount = 0;

	while(*ptr) {
		if(*ptr == '\n') enterCount++;
		ptr++;
	}

	char* copy = malloc(strlen(str) + enterCount + 1);
	char* copyPtr = copy;
	ptr = str;
	while(*ptr) {
		if(*ptr == '\n') {
			*copyPtr++ = '\\';
			*copyPtr++ = 'n';
			ptr++;
		}
		else *copyPtr++ = *ptr++;
	}
	*copyPtr = '\0';

	printf(format, copy);
	free(copy);
}


static void genStringLinks(String* str) {
	if(str->type == ST_BEGIN) {
		genStringLinks(str->next);
		return;
	}

	switch (str->type) {
		case ST_INTERP:
			printf("{");
			printf("\"type\": \"var\", \"var\": \"%s\"", str->id);
			printf("}");
			break;
		case ST_TEXT:
			printf("{");
			printf("\"type\": \"text\", \"text\": \"%s\"", str->text);
			printf("}");
			break;
		case ST_BEGIN:
			break;
		default:
			//TODO: Error
			break;
	}

	if (str->next != NULL) {
		printf(",");
		genStringLinks(str->next);
	}
}

static void genString(String* str) {
	printf("{");
	printf("\"type\": \"string\", \"links\": [");
	genStringLinks(str);
	printf("]}");
}

static void genConst(Constant* constant) {
	printf("{");
	
	if(constant->type == CT_BOOL) {
		printf("\"type\": \"bool\", \"value\": %s", constant->boolVal ? "true" : "false");
	} else if(constant->type == CT_NUM) {
		printf("\"type\": \"num\", \"value\": %f", constant->numVal);
	}

	printf("}");
}

static void genVal(Value* val) {
	switch(val->type) {
		case VT_CONST:
			genConst(val->constant);
			break;
		case VT_ID:
			printf("\"%s\"", val->id);
			break;
		case VT_STR:
			genString(val->str);
			break;
		default:
			//TODO: Error
			break;
	}
}

static void genFactor(Factor* factor) {
	printf("{");
	switch(factor->type) {
		case FT_VAL:
			if(factor->val->type == VT_ID) {
				printf("\"type\": \"var\", \"var\": ");
				genVal(factor->val);
			} else {
				printf("\"type\": \"val\", \"val\": ");
				genVal(factor->val);
			}
			break;
		case FT_EXPR:
			genExpr(factor->expr);
			break;
		default:
			break;
	}
	printf("}");
}

static void genExpr(Expression* expr) {
	printf("{");
	switch(expr->type) {
		case EXP_ADD:
			printf("\"type\": \"add\",");
			break;
		case EXP_SUB:
			printf("\"type\": \"sub\",");
			break;
		case EXP_MUL:
			printf("\"type\": \"mul\",");
			break;
		case EXP_DIV:
			printf("\"type\": \"div\",");
			break;
		case EXP_MOD:
			printf("\"type\": \"mod\",");
			break;
		case EXP_AND:
			printf("\"type\": \"and\",");
			break;
		case EXP_OR:
			printf("\"type\": \"or\",");
			break;
		case EXP_GE:
			printf("\"type\": \"ge\",");
			break;
		case EXP_GT:
			printf("\"type\": \"gt\",");
			break;
		case EXP_LE:
			printf("\"type\": \"le\",");
			break;
		case EXP_LT:
			printf("\"type\": \"lt\",");
			break;
		case EXP_IS:
			printf("\"type\": \"is\",");
			break;
		case EXP_NOT:
			printf("\"type\": \"not\",");
			break;
		case EXP_FACTOR:
			break;
		default:
			//TODO: Error
			break;
	}

	switch(expr->type) {
		case EXP_ADD:
		case EXP_SUB:
		case EXP_MUL:
		case EXP_DIV:
		case EXP_MOD:
		case EXP_AND:
		case EXP_OR:
		case EXP_GE:
		case EXP_GT:
		case EXP_LE:
		case EXP_LT:
		case EXP_IS:
			printf("\"left\": ");
			genExpr(expr->left);
			printf(",");
			printf("\"right\": ");
			genExpr(expr->right);
			break;
		case EXP_NOT:
			printf("\"expr\": ");
			genExpr(expr->left);
			break;
		case EXP_FACTOR:
			printf("\"value\": ");
			genFactor(expr->factor);
			break;
		default:
			//TODO: Error
			break;
	}
	printf("}");
}

static void genIfCond(IfCond* ifCond) {
	printf("{");

	printf("\"condition\": ");
	genExpr(ifCond->condition);

	printf(", \"trueVal\": ");
	genVal(ifCond->trueVal);

	printf(", \"falseVal\": ");
	genVal(ifCond->falseVal);

	printf("}");
}

static void genWhenThenStatements(WhenThen* whenThen) {
	printf("{");

	printf("\"when\": ");
	genConst(whenThen->constant);

	printf("\"then\": ");
	genVal(whenThen->val);

	printf("}");

	if(whenThen == NULL) {
		printf(",");
		genWhenThenStatements(whenThen->next);
	}
}

static void genMatchCond(MatchCond* matchCond) {
	printf("{");

	printf("\"var\": \"%s\",", matchCond->id);

	printf("\"whenThenStatements\": [");
	genWhenThenStatements(matchCond->whenThenStatements);
	printf("],");

	printf("\"default\": ");
	genVal(matchCond->defaultValue);

	printf("}");
}

static void genConditional(Conditional* cond) {
	switch(cond->type) {
		case CDT_IF:
			genIfCond(cond->ifCond);
			break;
		case CDT_MATCH:
			genMatchCond(cond->matchCond);
			break;
		default:
			//TODO: Error
			break;
	}
}

static void genValOrCond(ValOrCond* val) {
	if(val->type == VCT_VAL)
		genVal(val->value);
	else if (val->type == VCT_COND)
		genConditional(val->conditional);
	else {
		//TODO: ERROR
	}
}

static void genHeaderTags(Header* header) {	
	printf("\"%s\": ", header->id);
	genValOrCond(header->valOrCond);

	if(header->next != NULL) {
		printf(",");
		genHeaderTags(header->next);
	}
}

static void genHeader(Header* header) {
	printf("{");

	genHeaderTags(header);

	printf("}");
}

static void genFork(Fork* fork) {
	printf("{");

	printf("\"decision\": ");
	genVal(fork->val);

	printf("\"text\": ");
	genText(fork->text);

	printf("},");
}

static void genTriggerParams(TriggerParameter* params) {
	if(params == NULL) return;
	
	genExpr(params->expr);

	genTriggerParams(params->next);
}

static void genText(Text* text) {
	switch(text->type) {
		case TT_TEXT:
			printf("\"type\": \"text\",");
			printReplaceStr("\"text\": \"%s\"", text->text);
			break;
		case TT_BOLD:
			printf("\"bold\": true,");
			genText(text->inner);
			break;
		case TT_ITALIC:
			printf("\"italic\": true");
			genText(text->inner);
			break;
		case TT_INTERPVAR:
			printf("\"type\": \"var\",");
			printf("\"var\": \"%s\"", text->id);
			break;
		case TT_TRIGGER:
			printf("\"type\": \"trigger\",");
			printf("\"trigger\": \"%s\",", text->trigger->id);
			printf("\"params\": [");
			genTriggerParams(text->trigger->parameters);
			printf("]");
		default:
			break;
	}
}

static void genBodyLinks(Body* body) {
	printf("{");
	switch(body->type) {
		case BT_FORK:
			printf("\"type\": \"fork\",");
			genFork(body->fork);
			break;
		case BT_TEXT:
			genText(body->text);
			break;
		default:
			//TODO: Error
			break;
	}
	printf("}");

	if(body->next != NULL) {
		printf(",");
		genBodyLinks(body->next);
	}
}

static void genBody(Body* body) {
	printf("[");

	genBodyLinks(body);

	printf("]");
}

static void genBlock(Block* block) {
	printf("\"header\": ");
	genHeader(block->header);

	if(block->body != NULL) {
		printf(",");
		printf("\"body\": ");
		genBody(block->body);
	}
}

static void genBlocks(Program* program) {
	printf("{");
	genBlock(program->block);
	printf("}");
	
	if(program->next != NULL) {
		printf(",");
		genBlocks(program->next);
	}
}

void Generator(Program* program) {
	printf("[");
	genBlocks(program);
	printf("]\n");
}
