%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	Program* program;
	Block* block;
	Header* header;
	Body* body;
	Fork* fork;
	Expression* expression;
	Factor* factor;
	Constant* constant;
	Value* value;
	ValOrCond* valOrCond;
	Conditional* conditional;
	WhenThen* whenThen;
	Text* text;
	TaggedText* taggedText;
	String* stringStruct;
	Trigger* trigger;
	TriggerParameters* triggerParameters;

	// Terminales.
	token token;
	int integer;
	int boolean;
	char* string;
}

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV
%token <token> MOD

// Logic tokens
%token <token> AND
%token <token> OR
%token <token> NOT
%token <token> IS

// Conditional tokens
%token <token> IF
%token <token> ELSE
%token <token> MATCH
%token <token> WHEN
%token <token> THEN
%token <token> DEFAULT

// Comparation tokens
%token <token> LESS_THAN
%token <token> GREATER_THAN
%token <token> LESS_OR_EQUAL
%token <token> GREATER_OR_EQUAL

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> FORK
%token <token> BOLD
%token <token> ITALIC
%token <token> BEGIN_TAG
%token <token> CLOSE_TAG
%token <token> BEGIN_STRING
%token <token> END_STRING
%token <string> INTERP_VAR


%token <token> OPEN_BLOCK
%token <token> CLOSE_BLOCK
%token <token> SPLIT_BLOCK

%token <token> COLON
%token <string> IDENTIFIER

%token <integer> INTEGER
%token <boolean> BOOL
%token <string> STRING_TEXT
%token <string> TEXT

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <block> block
%type <constant> constant
%type <value> value
%type <valOrCond> value_with_conditional
%type <conditional> conditional
%type <factor> factor
%type <header> header
%type <body> body
%type <fork> fork
%type <text> text
%type <taggedText> tagged_text
%type <expression> expression
%type <whenThen> when_then
%type <stringStruct> string
%type <triggerParameters> trigger_parameters
%type <trigger> trigger

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV MOD

%left AND OR
%left NOT
%left LESS_THAN GREATER_THAN LESS_OR_EQUAL GREATER_OR_EQUAL
%left IS

// El símbolo inicial de la gramatica.
%start program

%%

program: block			{ $$ = EndBlockProgramGrammarAction($1); }
	| block program		{ $$ = BlockProgramGrammarAction($1, $2); }
	;

block: OPEN_BLOCK header SPLIT_BLOCK body CLOSE_BLOCK	{ $$ = BlockGrammarAction($2, $4); }
	;

header: %empty											{ $$ = EmptyHeaderGrammarAction(); }
	| IDENTIFIER COLON value_with_conditional header	{ $$ = HeaderItemGrammarAction($1, $3, $4); }
	;

body: %empty		{ $$ = EmptyBodyGrammarAction(); }
	| text body		{ $$ = TextBodyGrammarAction($1, $2); }
	| fork body		{ $$ = ForkBodyGrammarAction($1, $2); }
	;

fork: FORK value COLON text		{ $$ = ForkGrammarAction($2, $4); }
	;

text: BOLD text BOLD		{ $$ = BoldTextGrammarAction($2); }
	| ITALIC text ITALIC	{ $$ = ItalicTextGrammarAction($2); }
	| tagged_text			{ $$ = TaggedTextTextGrammarAction($1); }
	| INTERP_VAR			{ $$ = InterpVarTextGrammarAction($1); }
	| trigger				{ $$ = TriggerTextGrammarAction($1); }
	| TEXT					{ $$ = TextGrammarAcion($1); }
	;

tagged_text: BEGIN_TAG IDENTIFIER COLON value_with_conditional CLOSE_TAG OPEN_PARENTHESIS text CLOSE_PARENTHESIS	{ $$ = ValuedTaggedTextGrammarAction($4, $6); }
	| BEGIN_TAG IDENTIFIER CLOSE_TAG OPEN_PARENTHESIS text CLOSE_PARENTHESIS										{ $$ = UnvaluedTaggedTextGrammarAction($5); }
	;

expression: expression[left] ADD expression[right]			{ $$ = AddExprGrammarAction($left, $right); }
	| expression[left] SUB expression[right]				{ $$ = SubExprGrammarAction($left, $right); }
	| expression[left] MUL expression[right]				{ $$ = MulExprGrammarAction($left, $right); }
	| expression[left] DIV expression[right]				{ $$ = DivExprGrammarAction($left, $right); }
	| expression[left] MOD expression[right]				{ $$ = ModExprGrammarAction($left, $right); }
	| expression[left] IS expression[right]					{ $$ = IsExprGrammarAction($left, $right); }
	| expression[left] LESS_THAN expression[right]			{ $$ = LtExprGrammarAction($left, $right); }
	| expression[left] GREATER_THAN expression[right]		{ $$ = GtExprGrammarAction($left, $right); }
	| expression[left] LESS_OR_EQUAL expression[right]		{ $$ = LeExprGrammarAction($left, $right); }
	| expression[left] GREATER_OR_EQUAL expression[right]	{ $$ = GeExprGrammarAction($left, $right); }
	| expression[left] AND expression[right]				{ $$ = AndExprGrammarAction($left, $right); }
	| expression[left] OR expression[right]					{ $$ = OrExprGrammarAction($left, $right); }
	| NOT expression										{ $$ = NotExprGrammarAction($2); }
	| factor												{ $$ = FactorExprGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS	{ $$ = ExprFactorGrammarAction($2); }
	| value												{ $$ = ValFactorGrammarAction($1); }
	;

conditional: value IF expression ELSE value		{ $$ = IfCondGrammarAction($1, $3, $5); }
	| MATCH IDENTIFIER when_then DEFAULT value	{ $$ = MatchCondGrammarAction($2, $4); }
	;

when_then: %empty							{ $$ = EmptyWhenThenGrammarAction(); }
	| WHEN constant THEN value when_then	{ $$ = WhenThenGrammarAction($2, $4, $5); }			
	;

value_with_conditional: conditional		{ $$ = CondValOrCondGrammarAction($1); }
	| value								{ $$ = ValValOrCondGrammarAction($1);  }
	;

value: constant		{ $$ = ConstValueGrammarAction($1);  }
	| IDENTIFIER	{ $$ = IdValueGrammarAction($1); 	 }
	| string		{ $$ = StringValueGrammarAction($1); }
	;

string: BEGIN_STRING string	{ $$ = BeginStringGrammarAction($2); }
	| INTERP_VAR string		{ $$ = InterpVarStringGrammarAction($1, $2); }
	| STRING_TEXT string	{ $$ = TextStringGrammarAction($1, $2); }
	| END_STRING			{ $$ = EndStringGrammarAction(); }
	;

constant: INTEGER	{ $$ = IntConstantGrammarAction($1); }
	| BOOL			{ $$ = BoolConstantGrammarAction($1); }
	;

trigger: INTERP_VAR OPEN_PARENTHESIS trigger_parameters CLOSE_PARENTHESIS	{ $$ = TriggerGrammarAction($1, $3); }
	;

trigger_parameters: %empty	{ $$ = EmptyTriggerParametersGrammarAction(); }
	| expression			{ $$ = ExprTriggerParametersGrammarAction($1); }
	;

%%
