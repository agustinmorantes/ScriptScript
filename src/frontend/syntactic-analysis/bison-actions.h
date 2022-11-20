#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

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

//Conditionals
Conditional* MatchCondGrammarAction(char* id, WhenThen* statements, Value* val);
Conditional* IfCondGrammarAction(Value* trueVal, Expression* cond, Value* falseVal);

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
TaggedText* ValuedTaggedTextGrammarAction(char* id, ValOrCond* val, Text* text);
TaggedText* UnvaluedTaggedTextGrammarAction(char* id, Text* text);

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

#endif
