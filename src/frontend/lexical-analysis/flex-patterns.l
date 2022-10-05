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
alphanumeric    [{alpha}{digit}]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }

"{"											{ return OpenBlockPatternAction(yytext); }
<BLOCK_BODY>"}"								{ BEGIN(INITIAL); return CloseBlockPatternAction(yytext); }
"---\n"										{ BEGIN(BLOCK_BODY); return SplitBlockPatternAction(yytext); }
<BLOCK_BODY>${alpha}{alphanumeric}*				{ return VariablePatternAction(yytext) }
<BLOCK_BODY>\n@ {alphaDigit}*:				{ return BeginForkAnswerPatternAction(yytext, yyleng) }
<BLOCK_BODY>"\n"							{ return NewlinePatternAction(yytext); }
<BLOCK_BODY>\[{alphanumeric}*:?{alphanumeric}*\]\(		{ return BeginTagPatternAction(yytext, yyleng) }
<BLOCK_BODY>")"								{ return EndTagPatternAction(yytext) }
<BLOCK_BODY>\*								{ return BoldPatternAction(yytext, yyleng) }
<BLOCK_BODY>\*\*							{ return ItalicPatternAction(yytext, yyleng) }
<BLOCK_BODY>==							    { return HighlightPatternAction(yytext, yyleng) }
<BLOCK_BODY>[^\}]+							{ return TextPatternAction(yytext, yyleng); }

<BLOCK_BODY>"}"                             { BEGIN(INITIAL); return CloseBlockPatternAction(yytext); }
<BLOCK_BODY>[^ \}]+                         { return TextPatternAction(yytext, yyleng); }
<BLOCK_BODY>.                               { ; }

"{"                                         { return OpenBlockPatternAction(yytext); }
"}"                                         { return CloseBlockPatternAction(yytext); }
"---\n"                                     { BEGIN(BLOCK_BODY); return SplitBlockPatternAction(yytext); }

{alpha}{alphanumeric}*                      { return IdentifierPatternAction(yytext, yyleng); }
":"                                         { return ColonPatternAction(yytext); }

"+"											{ return AdditionOperatorPatternAction(yytext); }
"-"											{ return SubtractionOperatorPatternAction(yytext); }
"*"											{ return MultiplicationOperatorPatternAction(yytext); }
"/"											{ return DivisionOperatorPatternAction(yytext); }

"("											{ return OpenParenthesisPatternAction(yytext); }
")"											{ return CloseParenthesisPatternAction(yytext); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
