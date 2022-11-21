#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>

/**
 * Implementación de "bison-grammar.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext, yylineno);
	LogError("En ASCII es:");
	LogErrorRaw("\t");
	const int length = strlen(yytext);
	for (int i = 0; i < length; ++i) {
		LogErrorRaw("[%d]", yytext[i]);
	}
	LogErrorRaw("\n\n");
	state.errorCount++;
}

//Static functions -------------------------------------------------------------------------

static void setSymtableInterpVar(const char* id) {
	if(symtable_exists(id)) {
		SymtableEntry* entry = symtable_get(id);

		switch(entry->type) {
			case STT_ENTRYTYPE_VAR:
				break;
			case STT_ENTRYTYPE_TBD:
				entry->type = STT_ENTRYTYPE_VAR;
				break;
			case STT_ENTRYTYPE_BLOCK:
				yyerror("Se intentó utilizar un ID de bloque como una variable.");
				break;
			case STT_ENTRYTYPE_TRIGGER:
				yyerror("Se intentó utilizar un trigger como una variable.");
				break;
			default:
				yyerror("La variable fue utilizada previamente pero es de tipo desconocido. No debería suceder.");
				break;
		}
	} else {
		yyerror("El scanner no agregó el id a la tabla de símbolos. No debería suceder.");
	}
}

static void setSymtableTrigger(const char* id) {
	if(symtable_exists(id)) {
		SymtableEntry* entry = symtable_get(id);

		switch(entry->type) {
			case STT_ENTRYTYPE_TRIGGER:
				break;
			case STT_ENTRYTYPE_TBD:
				entry->type = STT_ENTRYTYPE_TRIGGER;
				break;
			case STT_ENTRYTYPE_VAR:
				yyerror("Se intentó utilizar una variable como un trigger.");
				break;
			case STT_ENTRYTYPE_BLOCK:
				yyerror("Se intentó utilizar un ID de bloque como un trigger.");
				break;
			default:
				yyerror("El id fue utilizado previamente pero es de tipo desconocido. No debería suceder.");
				break;
		}
	} else {
		yyerror("El scanner no agregó el id a la tabla de símbolos. No debería suceder.");
	}
}


// Expressions -----------------------------------------------------------------------------
static Expression* BinaryExpr(Expression* left, Expression* right, ExpressionType type) {
	Expression* expr = (Expression*) malloc(sizeof(Expression));
	expr->type = type;
	expr->left = left;
	expr->right = right;
	return expr;
}

Expression* AddExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tAddExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_ADD);
}

Expression* SubExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tSubExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_SUB);
}

Expression* MulExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tMulExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_MUL);
}

Expression* DivExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tDivExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_DIV);
}

Expression* ModExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tModExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_MOD);
}

Expression* IsExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tIsExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_IS);
}

Expression* LtExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tLtExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_LT);
}

Expression* GtExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tGtExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_GT);
}

Expression* LeExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tLeExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_LE);
}

Expression* GeExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tGeExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_GE);
}

Expression* AndExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tAndExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_AND);
}

Expression* OrExprGrammarAction(Expression* left, Expression* right) {
	LogDebug("\tOrExprGrammarAction(expr, expr)");
	return BinaryExpr(left, right, EXP_OR);
}

Expression* NotExprGrammarAction(Expression* expr) {
	LogDebug("\tNotExprGrammarAction(expr)");
	Expression* notExpr = (Expression*) malloc(sizeof(Expression));
	notExpr->type = EXP_NOT;
	notExpr->left = expr;
	notExpr->right = NULL;
	return notExpr;
}

Expression* FactorExprGrammarAction(Factor* factor) {
	LogDebug("\tFactorExprGrammarAction(factor)");
	Expression* expr = (Expression*) malloc(sizeof(Expression));
	expr->type = EXP_FACTOR;
	expr->factor = factor;
	expr->left = NULL;
	expr->right = NULL;
	return expr;
}
//Conditionals -----------------------------------------------------------------------------
Conditional* MatchCondGrammarAction(char* id, WhenThen* statements, Value* val) {
	LogDebug("\tMatchCondGrammarAction(id, statements, val)");
	symtable_add_with_type(id, STT_ENTRYTYPE_VAR);

	Conditional* cond = (Conditional*) malloc(sizeof(Conditional));
	cond->type = CDT_MATCH;
	cond->matchCond = (MatchCond*) malloc(sizeof(MatchCond));
	cond->matchCond->id = id;
	cond->matchCond->whenThenStatements = statements;
	cond->matchCond->defaultValue = val;
	return cond;
}

Conditional* IfCondGrammarAction(Value* trueVal, Expression* cond, Value* falseVal) {
	LogDebug("\tIfCondGrammarAction(trueVal, cond, falseVal)");
	Conditional* ifCond = (Conditional*) malloc(sizeof(Conditional));
	ifCond->ifCond = (IfCond*) malloc(sizeof(IfCond));
	
	ifCond->type = CDT_IF;
	ifCond->ifCond->trueVal = trueVal;
	ifCond->ifCond->condition = cond;
	ifCond->ifCond->falseVal = falseVal;
	return ifCond;
}

//Conditional Values -----------------------------------------------------------------------
ValOrCond* CondValOrCondGrammarAction(Conditional* cond) {
	LogDebug("\tCondValOrCondGrammarAction(cond)");
	ValOrCond* valOrCond = (ValOrCond*) malloc(sizeof(ValOrCond));
	valOrCond->type = VCT_COND;
	valOrCond->conditional = cond;
	return valOrCond;
}

ValOrCond* ValValOrCondGrammarAction(Value* val) {
	LogDebug("\tValValOrCondGrammarAction(val)");
	ValOrCond* valOrCond = (ValOrCond*) malloc(sizeof(ValOrCond));
	valOrCond->type = VCT_VAL;
	valOrCond->value = val;
	return valOrCond;
}

//Values -----------------------------------------------------------------------------------
Value* ConstValueGrammarAction(Constant* value) {
	LogDebug("\tConstValueGrammarAction(const)");
	Value* val = (Value*) malloc(sizeof(Value));
	val->type = VT_CONST;
	val->constant = value;
	return val;
}
Value* IdValueGrammarAction(char* id) {
	LogDebug("\tIdValueGrammarAction(id)");

	if(symtable_exists(id)) {
		SymtableEntry* entry = symtable_get(id);
		if(entry->type == STT_ENTRYTYPE_TBD)
			entry->type = STT_ENTRYTYPE_VAR;
	} else yyerror("El scanner no agregó la variable a la tabla de simbolos");

	Value* val = (Value*) malloc(sizeof(Value));
	val->type = VT_ID;
	val->id = id;
	return val;
}
Value* StringValueGrammarAction(String* string) {
	LogDebug("\tStringValueGrammarAction(string)");
	Value* val = (Value*) malloc(sizeof(Value));
	val->type = VT_STR;
	val->str = string;
	return val;
}

//Factors -----------------------------------------------------------------------------------
Factor* ExprFactorGrammarAction(Expression* expr) {
	LogDebug("\tExprFactorGrammarAction(expr)");
	Factor* factor = (Factor*) malloc(sizeof(Factor));
	factor->type = FT_EXPR;
	factor->expr = expr;
	return factor;
}
Factor* ValFactorGrammarAction(Value* val) {
	LogDebug("\tValFactorGrammarAction(val)");
	Factor* factor = (Factor*) malloc(sizeof(Factor));
	factor->type = FT_VAL;
	factor->val = val;
	return factor;
}

//Constants ---------------------------------------------------------------------------------
Constant* IntConstantGrammarAction(int value) {
	LogDebug("\tIntConstantGrammarAction(int)");
	Constant* constant = (Constant*) malloc(sizeof(Constant));
	constant->type = CT_NUM;
	constant->numVal = value;
	return constant;
}
Constant* BoolConstantGrammarAction(bool value) {
	LogDebug("\tBoolConstantGrammarAction(bool)");
	Constant* constant = (Constant*) malloc(sizeof(Constant));
	constant->type = CT_BOOL;
	constant->boolVal = value;
	return constant;
}

//Header ------------------------------------------------------------------------------------
Header* EmptyHeaderGrammarAction() {
	LogDebug("\tEmptyHeaderGrammarAction()");
	return NULL;
}
Header* HeaderItemGrammarAction(char* id, ValOrCond* item, Header* next) {
	LogDebug("\tHeaderItemGrammarAction(id, item, header)");

	if(strcmp(id, "id") == 0) {
		if(item->type == VCT_VAL && item->value->type == VT_ID) {
			if(symtable_exists(item->value->id)) {
				SymtableEntry* entry = symtable_get(item->value->id);
				
				if(entry->type == STT_ENTRYTYPE_BLOCK)
					yyerror("Hay múltiples bloques con el mismo ID.");
				else if(entry->type == STT_ENTRYTYPE_TBD)
					entry->type = STT_ENTRYTYPE_BLOCK;
				else if(entry->type == STT_ENTRYTYPE_VAR)
					entry->type = STT_ENTRYTYPE_BLOCK;
				else yyerror("Este ID ya se está utilizando para una variable o un trigger.");
			}
			else yyerror("El scanner no registró el ID. No debería suceder.");
		}
		else yyerror("Debe ser un id de bloque válido.");
	}

	symtable_add_with_type(id, STT_ENTRYTYPE_HEADER_TAG);

	Header* h = (Header*) malloc(sizeof(Header));
	h->id = id;
	h->valOrCond = item;
	h->next = next;
	return h;
}

//Body --------------------------------------------------------------------------------------
Body* EmptyBodyGrammarAction() {
	LogDebug("\tEmptyBodyGrammarAction()");
	return NULL;
}
Body* TextBodyGrammarAction(Text* text, Body* body) {
	LogDebug("\tTextBodyGrammarAction(text, body)");
	Body* b = (Body*) malloc(sizeof(Body));
	b->type = BT_TEXT;
	b->text = text;
	b->next = body;
	return b;
}
Body* ForkBodyGrammarAction(Fork* fork, Body* body) {
	LogDebug("\tForkBodyGrammarAction(fork, body)");
	Body* b = (Body*) malloc(sizeof(Body));
	b->type = BT_FORK;
	b->fork = fork;
	b->next = body;
	return b;
}

//Fork --------------------------------------------------------------------------------------
Fork* ForkGrammarAction(Value* val, Text* text) {
	LogDebug("\tForkGrammarAction(val, text)");
	Fork* fork = (Fork*) malloc(sizeof(Fork));
	fork->val = val;
	fork->text = text;
	return fork;
}

//Text --------------------------------------------------------------------------------------
Text* TextGrammarAction(char* text) {
	LogDebug("\tTextGrammarAction(text)");
	Text* t = (Text*) malloc(sizeof(Text));
	t->type = TT_TEXT;
	t->text = text;
	return t;
}
Text* TriggerTextGrammarAction(Trigger* trigger) {
	LogDebug("\tTriggerTextGrammarAction(trigger)");
	Text* t = (Text*) malloc(sizeof(Text));
	t->type = TT_TRIGGER;
	t->trigger = trigger;
	return t;
}
Text* InterpVarTextGrammarAction(char* id) {
	LogDebug("\tInterpVarTextGrammarAction(id)");

	setSymtableInterpVar(id);

	Text* t = (Text*) malloc(sizeof(Text));
	t->type = TT_INTERPVAR;
	t->id = id;
	return t;
}
Text* TaggedTextTextGrammarAction(TaggedText* val) {
	LogDebug("\tTaggedTextTextGrammarAction(taggedText)");
	Text* t = (Text*) malloc(sizeof(Text));
	t->type = TT_TAGGED;
	t->tagged = val;
	return t;
}
Text* ItalicTextGrammarAction(Text* text) {
	LogDebug("\tItalicTextGrammarAction(text)");
	Text* t = (Text*) malloc(sizeof(Text));
	t->type = TT_ITALIC;
	t->inner = text;
	return t;
}
Text* BoldTextGrammarAction(Text* text) {
	LogDebug("\tBoldTextGrammarAction(text)");
	Text* t = (Text*) malloc(sizeof(Text));
	t->type = TT_BOLD;
	t->inner = text;
	return t;
}

//Tagged Text --------------------------------------------------------------------------------------
TaggedText* ValuedTaggedTextGrammarAction(char* id, ValOrCond* val, Text* text) {
	LogDebug("\tValuedTaggedTextGrammarAction(val, text)");
	symtable_add_with_type(id, STT_ENTRYTYPE_TEXT_TAG);
	TaggedText* tagged = (TaggedText*) malloc(sizeof(TaggedText));
	tagged->id = id;
	tagged->val = val;
	tagged->text = text;
	return tagged;
}
TaggedText* UnvaluedTaggedTextGrammarAction(char* id, Text* text) {
	LogDebug("\tUnvaluedTaggedTextGrammarAction(text)");
	symtable_add_with_type(id, STT_ENTRYTYPE_TEXT_TAG);
	TaggedText* tagged = (TaggedText*) malloc(sizeof(TaggedText));
	tagged->id = id;
	tagged->val = NULL;
	tagged->text = text;
	return tagged;
}

//String --------------------------------------------------------------------------------------
String* BeginStringGrammarAction(String* next) {
	LogDebug("\tBeginStringGrammarAction(next)");
	String* str = (String*) malloc(sizeof(String));
	str->type = ST_BEGIN;
	str->next = next;
	return str;
}
String* InterpVarStringGrammarAction(char* id, String* next) {
	LogDebug("\tInterpVarStringGrammarAction(id, next)");

	setSymtableInterpVar(id);

	String* str = (String*) malloc(sizeof(String));
	str->type = ST_INTERP;
	str->id = id;
	str->next = next;
	return str;
}
String* TextStringGrammarAction(char* text, String* next) {
	LogDebug("\tTextStringGrammarAction(text, next)");
	String* str = (String*) malloc(sizeof(String));
	str->type = ST_TEXT;
	str->text = text;
	str->next = next;
	return str;
}
String* EndStringGrammarAction() {
	LogDebug("\tEndStringGrammarAction()");
	return NULL;
}

//When Then Statements --------------------------------------------------------------------------------------
WhenThen* EmptyWhenThenGrammarAction() {
	LogDebug("\tEmptyWhenThenGrammarAction()");
	return NULL;
}
WhenThen* WhenThenGrammarAction(Constant* constant, Value* val, WhenThen* next) {
	LogDebug("\tWhenThenGrammarAction(constant, val, next)");
	WhenThen* whenThen = (WhenThen*) malloc(sizeof(WhenThen));
	whenThen->constant = constant;
	whenThen->val = val;
	whenThen->next = next;
	return whenThen;
}

//Block --------------------------------------------------------------------------------------
Block* BlockGrammarAction(Header* header, Body* body) {
	LogDebug("\tBlockGrammarAction(header, body)");
	Block* block = (Block*) malloc(sizeof(Block));
	block->header = header;
	block->body = body;
	return block;
}

//Trigger --------------------------------------------------------------------------------------
Trigger* TriggerGrammarAction(char* id, TriggerParameter* parameters) {
	LogDebug("\tTriggerGrammarAction(id, parameters)");

	setSymtableTrigger(id);

	Trigger* trigger = (Trigger*) malloc(sizeof(Trigger));
	trigger->id = id;
	trigger->parameters = parameters;
	return trigger;
}
TriggerParameter* EmptyTriggerParametersGrammarAction() {
	LogDebug("\tEmptyTriggerParametersGrammarAction()");
	return NULL;
}
TriggerParameter* ExprTriggerParametersGrammarAction(Expression* expr) {
	LogDebug("\tExprTriggerParametersGrammarAction(expr)");
	TriggerParameter* param = (TriggerParameter*) malloc(sizeof(TriggerParameter));
	param->expr = expr;
	param->next = NULL;
	return param;
}


//Program --------------------------------------------------------------------------------------
Program* EndBlockProgramGrammarAction(Block* block) {
	LogDebug("\tEndBlockProgramGrammarAction(block)");
	Program* program = (Program*) malloc(sizeof(Program));
	program->block = block;
	program->next = NULL;

	if(state.errorCount == 0) {
		state.succeed = true;
		state.program = program;
	}

	return program;
}
Program* BlockProgramGrammarAction(Block* block, Program* next) {
	LogDebug("\tBlockProgramGrammarAction(block, next)");
	Program* program = (Program*) malloc(sizeof(Program));
	program->block = block;
	program->next = next;

	if(state.errorCount == 0) {
		state.succeed = true;
		state.program = program;
	}

	return program;
}
