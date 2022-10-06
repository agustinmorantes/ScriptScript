#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios multilínea.
void BeginCommentPatternAction();
void EndCommentPatternAction();

// Patrones terminales del lenguaje diseñado.
token AdditionOperatorPatternAction(const char * lexeme);
token CloseParenthesisPatternAction(const char * lexeme);
token DivisionOperatorPatternAction(const char * lexeme);
token IntegerPatternAction(const char * lexeme, const int length);
token MultiplicationOperatorPatternAction(const char * lexeme);
token OpenParenthesisPatternAction(const char * lexeme);
token SubtractionOperatorPatternAction(const char * lexeme);
token ModulusOperatorPatternAction(const char * lexeme);

token IsKeywordPatternAction(const char* lexeme); 
token NotKeywordPatternAction(const char* lexeme);
token AndKeywordPatternAction(const char* lexeme);
token OrKeywordPatternAction(const char* lexeme); 

token IfKeywordPatternAction(const char * lexeme);
token ElseKeywordPatternAction(const char * lexeme);
token MatchKeywordPatternAction(const char * lexeme);
token WhenKeywordPatternAction(const char * lexeme);
token ThenKeywordPatternAction(const char * lexeme);
token DefaultKeywordPatternAction(const char * lexeme);

token LessThanOperatorPatternAction(const char* lexeme);
token LessThanOrEqualOperatorPatternAction(const char* lexeme);
token GreaterThanOperatorPatternAction(const char* lexeme);
token GreaterThanOrEqualOperatorPatternAction(const char* lexeme);

token BoolPatternAction(const char * lexeme, const int length);

token OpenBlockPatternAction(const char * lexeme);
token CloseBlockPatternAction(const char * lexeme);
token SplitBlockPatternAction(const char * lexeme);
token IdentifierPatternAction(const char * lexeme, const int length);
token ColonPatternAction(const char * lexeme);

token TextCharacterPatternAction(const char * lexeme, const int length);
token InterpVariablePatternAction(const char * lexeme, const int length); 
token EndlinePatternAction(const char * lexeme);
token BeginForkPatternAction(const char * lexeme);
token BoldPatternAction(const char * lexeme);
token ItalicPatternAction(const char * lexeme);

token StringCharacterPatternAction(const char * lexeme, const int length);
token BeginStringPatternAction(const char * lexeme);
token EndStringPatternAction(const char * lexeme);

token BeginTagPatternAction(const char * lexeme);
token CloseTagPatternAction(const char * lexeme);

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);

#endif
