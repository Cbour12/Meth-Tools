#include "lexer.h"
#include "parse_tree_nodes.h"
#include <map>
#include <iostream>
#include <stdlib.h> 
#include <string>

using namespace std;

// Holds the symbols in the interpreted program
typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable;

extern int nextToken;        // next token returned by lexer
extern bool printParse;
extern bool printTree;
extern bool printDelete;

extern "C" {
	extern FILE *yyin;
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}


// Function declarations
ProgramNode* program();
BlockNode* block();
StatementNode* statement();
AssignmentNode* assignment();
CompoundNode* compound();
IfNode* If();
WhileNode* While();
WriteNode* Write();
ReadNode* Read();
FactorNode* factor();
TermNode* term();
Simple_expressionNode* simple_expression();
ExpressionNode* expression();

#endif /* PARSER_H */
