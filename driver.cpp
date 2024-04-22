#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"

using namespace std;

extern "C" {
    // Instantiate global variables
    extern FILE *yyin;     // input stream
    extern FILE *yyout;    // output stream
    extern int   yyleng;   // length of current lexeme
    extern char *yytext;   // text of current lexeme
    extern int   yylineno; // line number for current lexeme
    extern int   yylex();  // the generated lexical analyzer
    extern int   yylex_destroy();
}

// Needed global variables
extern int nextToken; // token returned from yylex

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[]) {

  // Set the input stream
  bool printTree = true; // whether to print the parse tree
  bool printSymbolTable = false;
  bool printParse = true;
  bool printsymbolTable = true;
   
  // Process any command-line switches
  for(int i = 1; i < argc; i++) {
    // -p flag: if requested, print while parsing
    if(strcmp(argv[i], "-p") == 0) {
      printParse = true;
    }
    // -t flag: if requested, print parse tree
    if(strcmp(argv[i], "-t") == 0) {
      printTree = true;
    }
    // -s flag
    if(strcmp(argv[i], "-s") == 0) {
      printSymbolTable = true;
    }
    // -d flag: if requested, print while deleting parse tree
    if(strcmp(argv[i], "-d") == 0) {
      printDelete = true;
    }
  }
  
  // Set the input stream
  if (argc > 1) {
      printf("INFO: Using the %s file for input\n", argv[argc-1]);
      yyin = fopen(argv[argc-1], "r");
      if (!yyin){
        perror("Error opening file");
        return(EXIT_FAILURE);
      } 
    }
    
    ProgramNode* root = nullptr;
    // Set the output stream
    yyout = stdout;

    // Get the first token
    nextToken = yylex();
    // Fire up the parser!
    try {
      // Process the expected production
      root = program();

    if (nextToken != TOK_EOF)
      throw "end of file expected, but there is more here!";
        
    } 
    catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near |" << yytext << ", error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }
    
    if (yyin)
      fclose(yyin);

    yylex_destroy();
    //-----------------------------------------------------------------------------------------
    //printing tree

    // Tell the world about our success!!
    cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;
  
    if(printTree) {
      cout << endl << "*** Print the Tree ***" << endl;
      cout << *root << endl << endl;
    }

    cout << "*** Interpret the Tree ***" << endl;
    root->interpret();

    // Print out the symbol table
    if(printSymbolTable)
    {
      cout << endl << "*** Print the Symbol Table ***" << endl;
      symbolTableT::iterator it;
      for (it = symbolTable.begin(); it != symbolTable.end(); ++it) 
        cout << setw(8) << it->first << ": " << it->second << endl;
    }
    cout << endl;

    if(printDelete)
      cout << "*** Delete the Tree ***" << endl;
    delete root;
    root = nullptr;

    return(EXIT_SUCCESS);
}
 
