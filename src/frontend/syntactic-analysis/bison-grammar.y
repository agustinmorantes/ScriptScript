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

%token <token> AND
%token <token> OR
%token <token> NOT
%token <token> IS

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
/* %type <program> program
%type <expression> expression
%type <factor> factor */
/* %type <constant> constant */

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

program: block		
	| block program	
	;

block: OPEN_BLOCK header SPLIT_BLOCK body CLOSE_BLOCK				{ printf("Block\n"); }
	;

header: %empty														{ printf("Header\n"); }
	| header_item header											{ printf("Header item\n"); }
	;

header_item: IDENTIFIER COLON value
	;

body: %empty														{ printf("Empty body\n"); }
	| text body														{ printf("body_text\n"); }
	| fork body														{ printf("body_fork\n"); }
	;

fork: FORK value COLON text
	;

text: TEXT
	| INTERP_VAR text
	| BOLD text BOLD
	| ITALIC text ITALIC
	| tagged_text
	;

tagged_text: BEGIN_TAG IDENTIFIER COLON value CLOSE_TAG OPEN_PARENTHESIS text CLOSE_PARENTHESIS
	| BEGIN_TAG IDENTIFIER CLOSE_TAG OPEN_PARENTHESIS text CLOSE_PARENTHESIS
	;

arith_expression: arith_expression[left] ADD arith_expression[right]
	| arith_expression[left] SUB arith_expression[right]
	| arith_expression[left] MUL arith_expression[right]
	| arith_expression[left] DIV arith_expression[right]
	| arith_expression[left] MOD arith_expression[right]
	| factor
	;

logical_expression: logical_expression[left] AND logical_expression[right]
	| logical_expression[left] OR logical_expression[right]
	| NOT logical_expression
	| value[left] IS value[right]
	| value[left] LESS_THAN value[right]
	| value[left] GREATER_THAN value[right]
	| value[left] LESS_OR_EQUAL value[right]
	| value[left] GREATER_OR_EQUAL value[right]
	| logical_factor
	;

logical_factor: OPEN_PARENTHESIS logical_expression CLOSE_PARENTHESIS
	| BOOL
    | IDENTIFIER
	;

factor: OPEN_PARENTHESIS arith_expression CLOSE_PARENTHESIS
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

%%
