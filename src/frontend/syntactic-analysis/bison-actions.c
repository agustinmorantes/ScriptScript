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
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
int ProgramGrammarAction(const int value) {
	LogDebug("\tProgramGrammarAction(%d)", value);
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	state.result = value;
	return value;
}

int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tAdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Add(leftValue, rightValue);
}

int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tSubtractionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Subtract(leftValue, rightValue);
}

int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tMultiplicationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Multiply(leftValue, rightValue);
}

int DivisionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tDivisionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Divide(leftValue, rightValue);
}

int FactorExpressionGrammarAction(const int value) {
	LogDebug("\tFactorExpressionGrammarAction(%d)", value);
	return value;
}

int ExpressionFactorGrammarAction(const int value) {
	LogDebug("\tExpressionFactorGrammarAction(%d)", value);
	return value;
}

int ConstantFactorGrammarAction(const int value) {
	LogDebug("\tConstantFactorGrammarAction(%d)", value);
	return value;
}

int IntegerConstantGrammarAction(const int value) {
	LogDebug("\tIntegerConstantGrammarAction(%d)", value);
	return value;
}

// Expressions
Expression* AddExprGrammarAction(Expression* left, Expression* right);
Expression* SubExprGrammarAction(Expression* left, Expression* right);
Expression* MulExprGrammarAction(Expression* left, Expression* right);
Expression* DivExprGrammarAction(Expression* left, Expression* right);
Expression* ModExprGrammarAction(Expression* left, Expression* right);
Expression* IsExprGrammarAction(Expression* left, Expression* right);
Expression* LtExprGrammarAction(Expression* left, Expression* right);
Expression* GtExprGrammarAction(Expression* left, Expression* right);
Expression* LeExprGrammarAction(Expression* left, Expression* right);
Expression* GeExprGrammarAction(Expression* left, Expression* right);
Expression* AndExprGrammarAction(Expression* left, Expression* right);
Expression* OrExprGrammarAction(Expression* left, Expression* right);
Expression* NotExprGrammarAction(Expression* expr);
Expression* FactorExprGrammarAction(Factor* factor);

//Conditional Values
ValOrCond* CondValOrCondGrammarAction(Conditional* cond);
ValOrCond* ValValOrCondGrammarAction(Value* val);

//Values
Value* ConstValueGrammarAction(Constant* value);
Value* IdValueGrammarAction(char* id);
Value* StringValueGrammarAction(String* string);

//Factors
Factor* ExprFactorGrammarAction(Expression* expr);
Factor* ValFactorGrammarAction(Value* val);

//Constants
Constant* IntConstantGrammarAction(int value);
Constant* BoolConstantGrammarAction(bool value);

//Header
Header* EmptyHeaderGrammarAction();
Header* HeaderItemGrammarAction(char* id, ValOrCond* item, Header* header);

//Body
Body* EmptyBodyGrammarAction();
Body* TextBodyGrammarAction(Text* text, Body* body);
Body* ForkBodyGrammarAction(Fork* fork, Body* body);

//Fork
Fork* ForkGrammarAction(Value* val, Text* text);

//Text
Text* TextGrammarAction(char* text);
Text* TriggerTextGrammarAction(Trigger* trigger);
Text* InterpVarTextGrammarAction(char* id);
Text* TaggedTextTextGrammarAction(TaggedText* val);
Text* ItalicTextGrammarAction(Text* text);
Text* BoldTextGrammarAction(Text* text);

//Tagged Text
TaggedText* ValuedTaggedTextGrammarAction(ValOrCond* val, Text* text);
TaggedText* UnvaluedTaggedTextGrammarAction(Text* text);

//String
String* BeginStringGrammarAction(String* next);
String* InterpVarStringGrammarAction(char* id, String* next);
String* TextStringGrammarAction(char* text, String* next);
String* EndStringGrammarAction();

//When Then Statements
WhenThen* EmptyWhenThenGrammarAction();
WhenThen* WhenThenGrammarAction(Constant* constant, Value* val, WhenThen* next);

//Block
Block* BlockGrammarAction(Header* header, Body* body);

//Trigger
Trigger* TriggerGrammarAction(char* id, TriggerParameter* parameters);
TriggerParameter* EmptyTriggerParametersGrammarAction();
TriggerParameter* ExprTriggerParametersGrammarAction(Expression* expr);


//Program
Program* EndBlockProgramGrammarAction(Block* block);
Program* BlockProgramGrammarAction(Block* block, Program* next);
