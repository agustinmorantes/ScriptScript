#include "analyzer.h"
#include "abstract-syntax-tree.h"
#include "../support/shared.h"
#include "../support/logger.h"
#include <stdlib.h>

// Bitflags/Bitmask. 
// Los valores del enum se utilizan para saber si 2 tipos son compatibles utilizando la operación binaria &.
typedef enum {
    T_NUM = 1,
    T_BOOL = 2,
    T_STR = 4,
    T_VAR = 7,
    T_BLOCK_ID = 8
} Type;

static Type typeFromUnion(Type t1, Type t2);
static Type typeConst(Constant* constant);
static Type typeVal(Value* val);
static Type typeFactor(Factor* factor);
static Type typeExpr(Expression* expr);
static Type typeIfCond(IfCond* ifCond);
static Type typeWhen(WhenThen* stmt);
static Type typeMatchCond(MatchCond* matchCond);
static Type typeCond(Conditional* conditional);
static Type typeValOrCond(ValOrCond * valOrCond);
static void analyzeHeader(Header* header);
static void analyzeFork(Fork* fork);
static void analyzeText(Text* text);
static void analyzeBody(Body* body);
static void analyzeBlock(Block* block);
static void analyzeProgram(Program* program);


static Type typeFromUnion(Type t1, Type t2) {
    if((t1 & t2) != 0) {
        if(t1 == T_VAR) return t2;
        if(t2 == T_VAR) return t1;
        return t1;
    }

    LogError("Incompatible type operation.");
    state.errorCount++;

    return t1;
}

static Type typeConst(Constant* constant) {
    switch(constant->type) {
        case CT_BOOL:
            return T_BOOL;
        case CT_NUM:
            return T_NUM;
        default:
            LogError("Type checker error");
            exit(1);
            break;
    }
}

static Type typeVal(Value* val) {
    switch(val->type) {
        case VT_CONST:
            return typeConst(val->constant);
        case VT_ID:
            SymtableEntry* entry = symtable_get(val->id);
            if(entry->type == STT_ENTRYTYPE_BLOCK) {
                return T_BLOCK_ID;
            }
            return T_VAR;
        case VT_STR:
            return T_STR;
        default:
            LogError("Type checker error");
            exit(1);
            break;
    }
}

static Type typeFactor(Factor* factor) {
    switch(factor->type) {
        case FT_EXPR:
            return typeExpr(factor->expr);
        case FT_VAL:
            return typeVal(factor->val);
        default:
            LogError("Type checker error");
            exit(1);
            break;
    }
}

static Type typeExpr(Expression* expr) {
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
            return typeFromUnion(typeExpr(expr->left), typeExpr(expr->right));
        case EXP_NOT:
            return typeExpr(expr->left);
        case EXP_FACTOR:
            return typeFactor(expr->factor);
        default:
            LogError("Type checker error");
            exit(1);
            break;
    }
}

static Type typeIfCond(IfCond* ifCond) {
    typeExpr(ifCond->condition);

    return typeFromUnion(typeVal(ifCond->trueVal), typeVal(ifCond->falseVal));
}

static Type typeWhen(WhenThen* stmt) {
    if(stmt->next == NULL) return typeVal(stmt->val);

    return typeFromUnion(typeVal(stmt->val), typeWhen(stmt->next));
}

static Type typeMatchCond(MatchCond* matchCond) {
    return typeFromUnion(typeVal(matchCond->defaultValue), typeWhen(matchCond->whenThenStatements));
}

static Type typeCond(Conditional* conditional) {
    if(conditional->type == CDT_IF)
        typeIfCond(conditional->ifCond);
    else if(conditional->type == CDT_MATCH)
        typeMatchCond(conditional->matchCond);
    else {
        LogError("Type checker error");
        exit(1);
    }
}

static Type typeValOrCond(ValOrCond * valOrCond) {
    if(valOrCond->type == VCT_VAL)
        return typeVal(valOrCond->value);
    if(valOrCond->type == VCT_COND)
        return typeCond(valOrCond->conditional);
    else {
        LogError("Type checker error");
        exit(1);
    }
}

static void analyzeHeaderItems(Header* header) {
    if(strcmp(header->id, "id") == 0 || strcmp(header->id, "next") == 0) {
        if(typeValOrCond(header->valOrCond) != T_BLOCK_ID) {
            LogError("El tag '%s' debe ser un ID de bloque válido.", header->id);
            state.errorCount++;
        }
    } else {
        typeValOrCond(header->valOrCond);
    }

    if(header->next != NULL)
        analyzeHeaderItems(header->next);
}

static void analyzeHeader(Header* header) {
    analyzeHeaderItems(header);
}

static void analyzeFork(Fork* fork) {
   typeVal(fork->val);
}

static void analyzeText(Text* text) {
    //Nothing to do
}

static void analyzeBody(Body* body) {
    switch (body->type)
    {
    case BT_FORK:
        analyzeFork(body->fork);
        break;
    case BT_TEXT:
        analyzeText(body->text);
        break;
    default:
        LogError("Type checker error");
        exit(1);
        break;
    }

    if(body->next != NULL)
        analyzeBody(body->next);
}

static void analyzeBlock(Block* block) {
    if(block->header != NULL)
        analyzeHeader(block->header);
    if(block->body != NULL)
        analyzeBody(block->body);
}

static void analyzeProgram(Program* program) {
    if(program->block != NULL)
        analyzeBlock(program->block);
    if(program->next != NULL)
        analyzeProgram(program->next);
}

void AnalyzeAST(Program * program) {
    analyzeProgram(program);
}