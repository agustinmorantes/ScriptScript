%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Habilita el stack de contextos de flex */
%option stack

/* Contextos de Flex. */
%x COMMENT
%x BODY
%x STRING

%s FORK

/* Patrones reutilizables. */
endline			[\n]
whitespace		[\ \f\n\r\t\v]
alpha           [a-zA-Z]
digit			[0-9]
alphanumeric    [a-zA-Z0-9]
identifier      {alpha}{alphanumeric}*

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }


<BODY>\\.							        { return TextCharacterPatternAction(&yytext[1], yyleng); }
<BODY>"}"								    { BEGIN(INITIAL); return CloseBlockPatternAction(yytext); }
<BODY>"@"               				    { BEGIN(FORK); return BeginForkPatternAction(yytext); }
<BODY>"["                                   { BEGIN(INITIAL); return BeginTagPatternAction(yytext); }
<BODY>"("									{ return OpenParenthesisPatternAction(yytext); }
<BODY>")"								    { return CloseParenthesisPatternAction(yytext); }
<BODY>\${identifier}          				{ return InterpVariablePatternAction(yytext,yyleng); }
<BODY>"**"								    { return BoldPatternAction(yytext); }
<BODY>"_"							        { return ItalicPatternAction(yytext); }
<BODY>[^}@\[\(\)\$\*\\\_]*                    { return TextCharacterPatternAction(yytext, yyleng); }
<BODY>.                                     { return UnknownPatternAction(yytext, yyleng); }


<FORK>":"                                   { BEGIN(BODY); return ColonPatternAction(yytext); }


\"                                          { yy_push_state(STRING); return BeginStringPatternAction(yytext); } 
<STRING>\"                                  { yy_pop_state(); return EndStringPatternAction(yytext); } 
<STRING>\\.                                 { return StringCharacterPatternAction(yytext, yyleng); }
<STRING>\${identifier}                      { return InterpVariablePatternAction(yytext,yyleng); }
<STRING>[^\"\n\$\\]*                          { return StringTextPatternAction(yytext, yyleng); }
<STRING>.                                   { return UnknownPatternAction(yytext, yyleng); }
<STRING>{endline}                           { return UnknownPatternAction(yytext, yyleng); }

"{"                                         { return OpenBlockPatternAction(yytext); }
---\n                                     { BEGIN(BODY); return SplitBlockPatternAction(yytext); }

":"                                         { return ColonPatternAction(yytext); }

"]"                                         { BEGIN(BODY); return CloseTagPatternAction(yytext); }

"+"											{ return AdditionOperatorPatternAction(yytext); }
"-"											{ return SubtractionOperatorPatternAction(yytext); }
"*"											{ return MultiplicationOperatorPatternAction(yytext); }
"/"											{ return DivisionOperatorPatternAction(yytext); }
"%"                                         { return ModulusOperatorPatternAction(yytext); }

"("											{ return OpenParenthesisPatternAction(yytext); }
")"											{ return CloseParenthesisPatternAction(yytext); }

"is"                                          { return IsKeywordPatternAction(yytext); }
"not"                                         { return NotKeywordPatternAction(yytext); }
"and"                                         { return AndKeywordPatternAction(yytext); }
"or"                                          { return OrKeywordPatternAction(yytext); }

"if"										{ return IfKeywordPatternAction(yytext); }
"else"										{ return ElseKeywordPatternAction(yytext); }
"match"										{ return MatchKeywordPatternAction(yytext); }
"when"										{ return WhenKeywordPatternAction(yytext); }
"then"										{ return ThenKeywordPatternAction(yytext); }
"default"								    { return DefaultKeywordPatternAction(yytext); }

("true"|"false")                            { return BoolPatternAction(yytext, yyleng); }
{digit}+									{ return IntegerPatternAction(yytext, yyleng); }
{identifier}                                { return IdentifierPatternAction(yytext, yyleng); }

"<"                                         { return LessThanOperatorPatternAction(yytext); }
"<="                                        { return LessThanOrEqualOperatorPatternAction(yytext); }
">"                                         { return GreaterThanOperatorPatternAction(yytext); }
">="                                        { return GreaterThanOrEqualOperatorPatternAction(yytext); }

{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
