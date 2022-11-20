#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/

typedef struct {
	Expression* expression;
} Program;


typedef enum {
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	FACTOR
} ExpressionType;

typedef struct Expression Expression;
struct Expression {
	ExpressionType type, leftType, rightType;
	union left {
		Expression* expr;
		Factor* factor;
	};
	union right {
		Expression* expr;
		Factor* factor;
	};
};

typedef struct {
	// TODO: String Item List
} String;

typedef enum {
	VT_CONST,
	VT_ID,
	VT_STR
} ValueType;
typedef struct {
	ValueType type;
	union value {
		Constant* constant;
		char* id;
		String* str;
	};
} Value;

typedef struct {
	Expression* condition;
	Value* trueVal;
	Value* falseVal;
} IfCond;

typedef struct {
	Constant* constant;
	Value* val;
	WhenThen* next;
} WhenThen;

typedef struct {
	char* id;
	WhenThen* whenThenStatements;
	Value* defaultValue;
} MatchCond;

typedef enum {
	CDT_IF,
	CDT_MATCH
} CondType;
typedef struct {
	CondType type;
	union cond {
		IfCond* ifCond;
		MatchCond* matchCond;
	};
} Conditional;


typedef enum {
	VCT_VAL,
	VCT_COND
} ValOrCondType;
typedef struct {
	ValOrCondType type;
	union content {
		Value* value;
		Conditional* conditional;
	};
} ValOrCond;

typedef enum {
	CT_NUM,
	CT_BOOL
} ConstantType;
typedef struct {
	ConstantType type;
	union value {
		float num;
		bool boolean;
	};
} Constant;


typedef enum {
	EXPRESSION,
	CONSTANT
} FactorType;
typedef struct {
	FactorType type;
	union value {
		Expression* expression;
		Constant* constant;
	};
} Factor;

typedef enum {
	TT_TEXT,
	TT_TRIGGER,
	TT_INTERPVAR,
	TT_TAGGED,
	TT_ITALIC,
	TT_BOLD
} TextType;
typedef struct {
	TextType type;
	union value {
		char* text;
		Trigger* trigger;
		char* id;
		TaggedText* tagged;
		Text* inner;
	};
} Text;

typedef struct {
	ValOrCond* val;
	Text* text;
} TaggedText;


typedef struct {
	Expression* expr;
	TriggerParameter* next;
} TriggerParameter;

typedef struct {
	char* id;
	TriggerParameter* parameters;
} Trigger;


typedef struct {
	Value* val;
	Text* text;
} Fork;


typedef struct {
	//TODO: HeaderItemList
} Header;

typedef struct {
	//TODO: BodyItemList
} Body;

typedef struct {
	Header* header;
	Body* body;
} Block;

#endif
