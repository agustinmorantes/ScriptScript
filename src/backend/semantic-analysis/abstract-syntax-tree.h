#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/

typedef struct {
	Block* block;
	Program* next;
} Program;


typedef enum {
	EXP_ADD,
	EXP_SUB,
	EXP_MUL,
	EXP_DIV,
	EXP_MOD,
	EXP_IS,
	EXP_LT,
	EXP_GT,
	EXP_LE,
	EXP_GE,
	EXP_AND,
	EXP_OR,
	EXP_NOT,
	EXP_FACTOR
} ExpressionType;

typedef struct Expression Expression;
struct Expression {
	ExpressionType type;
	Expression* left;
	Expression* right;
	Factor* factor;
};


typedef enum {
	ST_BEGIN,
	ST_INTERP,
	ST_TEXT
} StringType;
typedef struct {
	StringType* type;

	union {
		char* id;
		char* text;
	};

	String* next;
} String;

typedef enum {
	VT_CONST,
	VT_ID,
	VT_STR
} ValueType;
typedef struct {
	ValueType type;
	union {
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
	union {
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
	union {
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
	union {
		float num;
		bool boolean;
	};
} Constant;


typedef enum {
	FT_EXPR,
	FT_VAL
} FactorType;
typedef struct {
	FactorType type;
	union {
		Expression* expr;
		Value* val;
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
	union {
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
	char* id;
	ValOrCond* valOrCond;
	Header* next;
} Header;

typedef enum {
	BT_TEXT,
	BT_FORK
} BodyType;
typedef struct {
	BodyType type;
	union {
		Text* text;
		Fork* fork;
	};

	Body* next;
} Body;

typedef struct {
	Header* header;
	Body* body;
} Block;

#endif
