#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include "parse_tree_nodes.h"

using namespace std;

extern bool printDelete;

class ProgramNode;
class BlockNode;
class StatementNode;
class AssignmentNode;
class CompoundNode;
class IfNode;
class WhileNode;
class ReadNode;
class WriteNode;
class ExpressionNode;
class Simple_expressionNode;
class TermNode;
class FactorNode;

//-----------------------------------------------------------------
//Bottom up
class FactorNode
{
public:
    int _level = 0;
    virtual float interpret() = 0;
    virtual void printTo(ostream &os) = 0;
    virtual ~FactorNode();
	                                        };
ostream& operator<<(ostream&, FactorNode&);
// ---------------------------------------------------------------------------
// subclasses in factor
class IdNode : public FactorNode{
public:
    string id;

    IdNode(int level, string name);
    ~IdNode();
    void printTo(ostream & os);
    float interpret();
};

class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int level, int value);
    ~IntLitNode();
    void printTo(ostream & os);
    float interpret();
};

class FloatLitNode : public FactorNode {
public:
    float float_literal = 0;

    FloatLitNode(int level, float value);
    ~FloatLitNode();
    void printTo(ostream & os);
    float interpret();
};

class NestedExprNode : public FactorNode {
public:
    ExpressionNode* exprPtr = nullptr; 

    NestedExprNode(int level, ExpressionNode* en);
    void printTo(ostream & os);
    ~NestedExprNode();
    float interpret();
};

class NotNode : public FactorNode {
public:
    
    FactorNode* factPtr = nullptr;
    NotNode(int level, FactorNode* factor);
    ~NotNode();
    void printTo(ostream &os);
    float interpret();
};

class MinusNode : public FactorNode {
public:

    FactorNode* factPtr = nullptr;
    MinusNode(int level, FactorNode* factor);
    ~MinusNode();
    void printTo(ostream &os);
    float interpret();
};
//----------------------------------------------------------------
class TermNode {
public:
  int _level = 0;              // recursion level of this node
  FactorNode* firstFactor = nullptr;
  vector<int> restFactorOps;   // TOK_MULT_OP or TOK_DIV_OP
  vector<FactorNode*> restFactors;

  TermNode(int level, FactorNode* firstFactor);
  ~TermNode();
  float interpret();
};
ostream& operator<<(ostream&, TermNode&); // Node print operator
//--------------------------------------------------------------
class Simple_expressionNode {
public:
  int _level = 0;          // recursion level of this node
  TermNode* firstTerm = nullptr;
  vector<int> restTermOps; // TOK_ADD_OP or TOK_SUB_OP
  vector<TermNode*> restTerms;

  Simple_expressionNode(int level, TermNode* firstTerm);
  ~Simple_expressionNode();
  float interpret();
};
ostream& operator<<(ostream&, Simple_expressionNode&); // Node print operator
//-----------------------------------------------------------------
class ExpressionNode{
public:
    int _level = 0;
    Simple_expressionNode* firstSimpleExpr = nullptr;
    int simpleExprOps = 0;
    Simple_expressionNode* secondSimpleExpr = nullptr;

    ExpressionNode(int level, Simple_expressionNode* firstSimpleExpr);
    ~ExpressionNode();
    float interpret();
};
ostream& operator<<(ostream&, ExpressionNode&);
//----------------------------------------------------------------
//Top down
class ProgramNode{
public: 
    int _level = 0;
    BlockNode* block = nullptr;

    ProgramNode(int level, BlockNode* Block);
    ~ProgramNode();
    void interpret();
};
ostream& operator<<(ostream&, ProgramNode&);
//---------------------------------------------------------------
class BlockNode{
public:
    int _level = 0;
    CompoundNode* _compound = nullptr;
    
    BlockNode(int level, CompoundNode* compound);
    ~BlockNode();
    void interpret();
};
ostream& operator<<(ostream&, BlockNode&);
//--------------------------------------------------------------
class StatementNode{
public:
    int _level = 0;
    virtual void interpret();
    virtual void printTo(ostream &os) = 0;
    virtual ~StatementNode();
};
ostream& operator<<(ostream&, StatementNode&);
//--------------------------------------------------------------
class AssignmentNode : public StatementNode{
public:
    string ident;
    ExpressionNode* expression = nullptr;
    
    AssignmentNode(int level);
    ~AssignmentNode();
    void printTo(ostream &os);
    void interpret();
};
//-------------------------------------------------------------
class IfNode : public StatementNode{
public:
   
    ExpressionNode* expression = nullptr;
    StatementNode*  then_stmt = nullptr;
    StatementNode*  else_stmt = nullptr;
    
    IfNode(int level);
    ~IfNode();
    void printTo(ostream &os);
    void interpret();
};
//-------------------------------------------------------------
class WhileNode : public StatementNode{
public:    

    ExpressionNode* expression = nullptr;
    StatementNode* statement = nullptr;
    
    WhileNode(int level);
    ~WhileNode();
    void printTo(ostream &os);
    void interpret();
};
//---------------------------------------------------------------
class ReadNode : public StatementNode{
public:
    string READ;
    
    ReadNode(int level);
    ~ReadNode();
    void printTo(ostream &os);
    void interpret();
};
//----------------------------------------------------------------
class WriteNode : public StatementNode{
public:    
    int type = 0;
    string content;
    
    WriteNode(int level);
    ~WriteNode();
    void printTo(ostream &os);
    void interpret();
};
//-----------------------------------------------------------------
class CompoundNode : public StatementNode{
public:
    vector<StatementNode*> Statements;

    CompoundNode(int level);
    ~CompoundNode();
    void printTo(ostream &os);
    void interpret();
};
//-----------------------------------------------------------------
#endif
