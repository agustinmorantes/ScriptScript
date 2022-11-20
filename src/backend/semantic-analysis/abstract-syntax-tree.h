#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/


typedef struct Constant Constant;
typedef struct String String;
typedef struct Expression Expression;
typedef struct WhenThen WhenThen;
typedef struct Trigger Trigger;
typedef struct TaggedText TaggedText;


typedef enum {
	CT_NUM,
	CT_BOOL
} ConstantType;
struct Constant {
	ConstantType type;
	union {
		float num;
		bool boolean;
	};
};

typedef enum {
	ST_BEGIN,
	ST_INTERP,
	ST_TEXT
} StringType;
struct String {
	StringType type;

	union {
		char* id;
		char* text;
	};

	String* next;
};

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

struct WhenThen {
	Constant* constant;
	Value* val;
	WhenThen* next;
};

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
		IfCond ifCond;
		MatchCond matchCond;
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

struct Expression {
	ExpressionType type;
	Expression* left;
	Expression* right;
	Factor* factor;
};

typedef enum {
	TT_TEXT,
	TT_TRIGGER,
	TT_INTERPVAR,
	TT_TAGGED,
	TT_ITALIC,
	TT_BOLD
} TextType;
typedef struct Text {
	TextType type;
	union {
		char* text;
		Trigger* trigger;
		char* id;
		TaggedText* tagged;
		struct Text* inner;
	};
} Text;

struct TaggedText {
	char* id;
	ValOrCond* val;
	Text* text;
};


typedef struct TriggerParameter {
	Expression* expr;
	struct TriggerParameter* next;
} TriggerParameter;

struct Trigger {
	char* id;
	TriggerParameter* parameters;
};


typedef struct {
	Value* val;
	Text* text;
} Fork;


typedef struct Header {
	char* id;
	ValOrCond* valOrCond;
	struct Header* next;
} Header;

typedef enum {
	BT_TEXT,
	BT_FORK
} BodyType;
typedef struct Body {
	BodyType type;
	union {
		Text* text;
		Fork* fork;
	};

	struct Body* next;
} Body;

typedef struct {
	Header* header;
	Body* body;
} Block;

typedef struct Program {
	Block* block;
	struct Program* next;
} Program;

#endif
