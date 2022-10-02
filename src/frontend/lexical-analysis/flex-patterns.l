%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT
%x BLOCK_BODY

/* Patrones reutilizables. */
digit			[0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]
alpha           [a-zA-Z]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

<BLOCK_BODY>"}"                             { BEGIN(INITIAL); return CloseBlockPatternAction(yytext); }
<BLOCK_BODY>[^ \}]+                         { return TextPatternAction(yytext, yyleng); }

"{"                                         { return OpenBlockPatternAction(yytext); }
"}"                                         { return CloseBlockPatternAction(yytext); }
"---\n"                                     { BEGIN(BLOCK_BODY); return SplitBlockPatternAction(yytext); }



"+"											{ return AdditionOperatorPatternAction(yytext); }
"-"											{ return SubtractionOperatorPatternAction(yytext); }
"*"											{ return MultiplicationOperatorPatternAction(yytext); }
"/"											{ return DivisionOperatorPatternAction(yytext); }
"%"                                         { return ModulusOperatorPatternAction(yytext); }

"("											{ return OpenParenthesisPatternAction(yytext); }
")"											{ return CloseParenthesisPatternAction(yytext); }

"is"                                        { return IsKeywordPatternAction(yytext); }
"not"                                       { return NotKeywordPatternAction(yytext); }
"and"                                       { return AndKeywordPatternAction(yytext); }
"or"                                        { return OrKeywordPatternAction(yytext); }

"<"                                         { return LessThanOperatorPatternAction(yytext); }
"<="                                        { return LessThanOrEqualOperatorPatternAction(yytext); }
">"                                         { return GreaterThanOperatorPatternAction(yytext); }
">="                                        { return GreaterThanOrEqualOperatorPatternAction(yytext); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
