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

%token <integer> INTEGER

%token <token> OPEN_BLOCK
%token <token> CLOSE_BLOCK
%token <token> SPLIT_BLOCK

%token <integer> TEXT

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <expression> expression
%type <factor> factor
%type <constant> constant

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV

%left AND OR
%left NOT
%left IS

// El símbolo inicial de la gramatica.
%start program

%%

program: expression													{ $$ = ProgramGrammarAction($1); }
	| boolean_expression											{ $$ = ProgramGrammarAction(0); }
	;

boolean_expression: boolean_expression[left] IS boolean_expression[right]			{ printf("is; "); }
	| boolean_expression[left] AND boolean_expression[right]						{ printf("and; ");  }
	| boolean_expression[left] OR boolean_expression[right]							{ printf("or; ");  }
	| NOT boolean_expression														{ printf("not; ");  }
	| relational_expression
	;

relational_expression: expression[left] LESS_THAN expression[right]	{ printf("%d < %d; ", $left, $right); }
	| expression[left] GREATER_THAN expression[right]				{ printf("%d > %d; ", $left, $right); }
	| expression[left] LESS_OR_EQUAL expression[right]				{ printf("%d <= %d; ", $left, $right); }
	| expression[left] GREATER_OR_EQUAL expression[right]			{ printf("%d >= %d; ", $left, $right); }
	;

expression: expression[left] ADD expression[right]					{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| factor														{ $$ = FactorExpressionGrammarAction($1); }
	| block															{ $$ = 0; }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	;

block: OPEN_BLOCK CLOSE_BLOCK										{ printf("Empty block\n"); }
	|  OPEN_BLOCK SPLIT_BLOCK CLOSE_BLOCK							{ printf("Empty split block\n"); }
	;
%%
