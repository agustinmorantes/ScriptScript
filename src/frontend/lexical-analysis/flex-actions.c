#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

void BeginCommentPatternAction() {
	LogDebug("BeginCommentPatternAction.");
}

void EndCommentPatternAction() {
	LogDebug("EndCommentPatternAction.");
}

token AdditionOperatorPatternAction(const char * lexeme) {
	LogDebug("AdditionOperatorPatternAction: '%s'.", lexeme);
	yylval.token = ADD;
	return ADD;
}

token CloseParenthesisPatternAction(const char * lexeme) {
	LogDebug("CloseParenthesisPatternAction: '%s'.", lexeme);
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token DivisionOperatorPatternAction(const char * lexeme) {
	LogDebug("DivisionOperatorPatternAction: '%s'.", lexeme);
	yylval.token = DIV;
	return DIV;
}

token IntegerPatternAction(const char * lexeme, const int length) {
	LogDebug("IntegerPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.integer = atoi(lexeme);
	return INTEGER;
}

token MultiplicationOperatorPatternAction(const char * lexeme) {
	LogDebug("MultiplicationOperatorPatternAction: '%s'.", lexeme);
	yylval.token = MUL;
	return MUL;
}

token OpenParenthesisPatternAction(const char * lexeme) {
	LogDebug("OpenParenthesisPatternAction: '%s'.", lexeme);
	yylval.token = OPEN_PARENTHESIS;
	return OPEN_PARENTHESIS;
}

token SubtractionOperatorPatternAction(const char * lexeme) {
	LogDebug("SubtractionOperatorPatternAction: '%s'.", lexeme);
	yylval.token = SUB;
	return SUB;
}

token UnknownPatternAction(const char * lexeme, const int length) {
	LogDebug("UnknownPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.token = YYUNDEF;
	// Al emitir este token, el compilador aborta la ejecución.
	return YYUNDEF;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	LogDebug("IgnoredPatternAction: '%s' (length = %d).", lexeme, length);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
}

token OpenBlockPatternAction(const char * lexeme) {
	LogDebug("OpenBlockPatternAction: '%s'.", lexeme);
	yylval.token = OPEN_BLOCK;
	return OPEN_BLOCK;
}

token CloseBlockPatternAction(const char * lexeme) {
	LogDebug("CloseBlockPatternAction: '%s'.", lexeme);
	yylval.token = CLOSE_BLOCK;
	return CLOSE_BLOCK;
}

token SplitBlockPatternAction(const char * lexeme) {
	LogDebug("SplitBlockPatternAction: '%s'.", lexeme);
	yylval.token = SPLIT_BLOCK;
	return SPLIT_BLOCK;
}

token IdentifierPatternAction(const char * lexeme, const int length) {
	LogDebug("IdentifierPatternAction", lexeme);
	yylval.token = IDENTIFIER;
	return IDENTIFIER;
}

token ColonPatternAction(const char * lexeme) {
	LogDebug("ColonPatternAction", lexeme);
	yylval.token = COLON;
	return COLON;
}

token TextCharacterPatternAction(const char * lexeme, const int length) {
	char c = length == 1 ? lexeme[0] : lexeme[1];
	LogDebug("%c", c);
	//TODO: Guardar bien el texto
	yylval.character = length;
	return TEXT;
}

token ModulusOperatorPatternAction(const char * lexeme) {
	LogDebug("ModulusOperatorPatternAction: '%s'.", lexeme);
	yylval.token = MOD;
	return MOD;
}

token IsKeywordPatternAction(const char* lexeme) {
	LogDebug("IsKeywordPatternAction: '%s'.", lexeme);
	yylval.token = IS;
	return IS;
}

token NotKeywordPatternAction(const char* lexeme) {
	LogDebug("NotKeywordPatternAction: '%s'.", lexeme);
	yylval.token = NOT;
	return NOT;
}

token AndKeywordPatternAction(const char* lexeme) {
	LogDebug("AndKeywordPatternAction: '%s'.", lexeme);
	yylval.token = AND;
	return AND;
}

token OrKeywordPatternAction(const char* lexeme) {
	LogDebug("OrKeywordPatternAction: '%s'.", lexeme);
	yylval.token = OR;
	return OR;
}

token LessThanOperatorPatternAction(const char* lexeme) {
	LogDebug("LessThanOperatorPatternAction: '%s'.", lexeme);
	yylval.token = LESS_THAN;
	return LESS_THAN;
}

token LessThanOrEqualOperatorPatternAction(const char* lexeme) {
	LogDebug("LessThanOrEqualOperatorPatternAction: '%s'.", lexeme);
	yylval.token = LESS_OR_EQUAL;
	return LESS_OR_EQUAL;
}

token GreaterThanOperatorPatternAction(const char* lexeme) {
	LogDebug("GreaterThanOperatorPatternAction: '%s'.", lexeme);
	yylval.token = GREATER_THAN;
	return GREATER_THAN;
}

token GreaterThanOrEqualOperatorPatternAction(const char* lexeme) {
	LogDebug("GreaterThanOrEqualOperatorPatternAction: '%s'.", lexeme);
	yylval.token = GREATER_OR_EQUAL;
	return GREATER_OR_EQUAL;
}

token InterpVariablePatternAction(const char * lexeme, const int length) {
	LogDebug("InterpVariablePatternAction", &lexeme[1]);
	//TODO: Guardar bien el texto
	yylval.token = INTERP_VAR;
	return INTERP_VAR;
}

token BeginForkPatternAction(const char * lexeme) {
	LogDebug("BeginForkPatternAction", lexeme);
	yylval.token = FORK;
	return FORK;
}

token BoldPatternAction(const char * lexeme) {
	LogDebug("BoldPatternAction", lexeme);
	yylval.token = BOLD;
	return BOLD;
}

token ItalicPatternAction(const char * lexeme) {
	LogDebug("ItalicPatternAction", lexeme);
	yylval.token = ITALIC;
	return ITALIC;
}

token StringCharacterPatternAction(const char * lexeme, const int length) {
	char c = length == 1 ? lexeme[0] : lexeme[1];
	LogDebug("%c", c);
	yylval.character = c;
	return STRING_TEXT;
}

token BeginStringPatternAction(const char * lexeme) {
	LogDebug("BeginStringPatternAction", lexeme);
	yylval.token = BEGIN_STRING;
	return BEGIN_STRING;
}

token EndStringPatternAction(const char * lexeme) {
	LogDebug("EndStringPatternAction", lexeme);
	yylval.token = END_STRING;
	return END_STRING;
}

token BeginTagPatternAction(const char * lexeme) {
	LogDebug("BeginTagPatternAction", lexeme);
	yylval.token = BEGIN_TAG;
	return BEGIN_TAG;
}

token CloseTagPatternAction(const char * lexeme) {
	LogDebug("CloseTagPatternAction", lexeme);
	yylval.token = CLOSE_TAG;
	return CLOSE_TAG;
}

token BoolPatternAction(const char * lexeme, const int length) {
	int val = length == 4 ? 1 : 0;
	LogDebug("BoolPatternAction: %d", val);
	yylval.boolean = val;
	return BOOL;
}
