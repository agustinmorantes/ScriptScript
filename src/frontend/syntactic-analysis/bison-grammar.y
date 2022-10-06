%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int expression;
	int factor;
	int constant;

	// Terminales.
	token token;
	int integer;
	int boolean;
	char character;
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
%token <token> INTERP_VAR


%token <token> OPEN_BLOCK
%token <token> CLOSE_BLOCK
%token <token> SPLIT_BLOCK

%token <token> COLON
%token <token> IDENTIFIER

%token <integer> INTEGER
%token <boolean> BOOL
%token <character> STRING_TEXT
%token <character> TEXT

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program

/* %type <expression> expression
%type <factor> factor
%type <constant> constant */

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

program: block			{ $$ = ProgramGrammarAction(0); }
	| block program		{ $$ = ProgramGrammarAction(0); }
	;

block: OPEN_BLOCK header SPLIT_BLOCK body CLOSE_BLOCK				{ printf("block\n"); }
	;

header: %empty														{ printf("header\n"); }
	| header_item header											{ printf("header_item\n"); }
	;

header_item: IDENTIFIER COLON value_with_conditional
	;

body: %empty														{ printf("body\n"); }
	| text body
	| fork body
	;

fork: FORK value COLON text
	;

text: BOLD text BOLD
	| ITALIC text ITALIC
	| tagged_text
	| INTERP_VAR
	| trigger
	| TEXT
	;

tagged_text: BEGIN_TAG IDENTIFIER COLON value_with_conditional CLOSE_TAG OPEN_PARENTHESIS text CLOSE_PARENTHESIS
	| BEGIN_TAG IDENTIFIER CLOSE_TAG OPEN_PARENTHESIS text CLOSE_PARENTHESIS
	;

expression: expression[left] ADD expression[right]
	| expression[left] SUB expression[right]
	| expression[left] MUL expression[right]
	| expression[left] DIV expression[right]
	| expression[left] MOD expression[right]
	| expression[left] IS expression[right]
	| expression[left] LESS_THAN expression[right]
	| expression[left] GREATER_THAN expression[right]
	| expression[left] LESS_OR_EQUAL expression[right]
	| expression[left] GREATER_OR_EQUAL expression[right]
	| expression[left] AND expression[right]
	| expression[left] OR expression[right]
	| NOT expression
	| factor
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
	| value
	;

conditional: value IF expression ELSE value				
	| MATCH IDENTIFIER when_then DEFAULT value						
	;

when_then: %empty
	| WHEN constant THEN value when_then													
	;

value_with_conditional: conditional
	| value
	;

value: constant
	| IDENTIFIER
	| string
	;

string: BEGIN_STRING string
	| INTERP_VAR string
	| STRING_TEXT string
	| END_STRING
	;

constant: INTEGER
	| BOOL
	;

trigger: INTERP_VAR OPEN_PARENTHESIS trigger_parameters CLOSE_PARENTHESIS
	;

trigger_parameters: %empty
	| expression
	; 

%%
