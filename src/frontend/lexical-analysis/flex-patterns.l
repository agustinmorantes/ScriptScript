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
al_dig          [a-zA-Z0-9]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }

"{"											{ return OpenBlockPatternAction(yytext); }
<BLOCK_BODY>"}"								{ BEGIN(INITIAL); return CloseBlockPatternAction(yytext); }
"---\n"										{ BEGIN(BLOCK_BODY); return SplitBlockPatternAction(yytext); }
<BLOCK_BODY>${alpha}{al_dig}*				{ return VariablePatternAction(yytext) }
<BLOCK_BODY>\n@ {alphaDigit}*:				{ return BeginForkAnswerPatternAction(yytext, yyleng) }
<BLOCK_BODY>"\n"							{ return NewlinePatternAction(yytext); }
<BLOCK_BODY>\[{al_dig}*:?{al_dig}*\]\(		{ return BeginTagPatternAction(yytext, yyleng) }
<BLOCK_BODY>")"								{ return EndTagPatternAction(yytext) }
<BLOCK_BODY>\*								{ return BoldPatternAction(yytext, yyleng) }
<BLOCK_BODY>\*\*							{ return ItalicPatternAction(yytext, yyleng) }
<BLOCK_BODY>==							    { return HighlightPatternAction(yytext, yyleng) }
<BLOCK_BODY>[^\}]+							{ return TextPatternAction(yytext, yyleng); }

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
