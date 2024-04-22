#define EPSILON 0.001
#include "parse_tree_nodes.h"
#include "parser.h"

bool printDelete = true;

static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("| ");
}

static bool truth(float F){
  return !((EPSILON > F) && (F > -EPSILON));
}
 //------------------------------------------------------------
FactorNode::~FactorNode() {}

ostream& operator<<(ostream& os, FactorNode& fn){
    os << endl; indent(fn._level); os << "(factor ";
    fn.printTo(os);
    os << endl; indent(fn._level); os << "factor)";
    return os;
}
float FactorNode::interpret()
{
  return this->interpret();
}
//----------------------------------------------------------
IdNode::IdNode(int level, string name) {
    _level = level - 1;
    id = string(name);
}
IdNode::~IdNode() {
    if(printDelete)
      cout << "Deleting FactorNode:IdNode" << endl;
    
}
void IdNode::printTo(ostream& os) {
    os << "( IDENT: " << id << " )";
}
float IdNode::interpret()
{
  symbolTableT::iterator variable = symbolTable.find(id);
  
  return variable->second;
}
//------------------------------------------------------------
IntLitNode::IntLitNode(int level, int value) {
  _level = level - 1;
  int_literal = value;
}
IntLitNode::~IntLitNode(){
  if(printDelete)
    cout << "Deleting FactorNode:IntLitNode " << endl; 
}
void IntLitNode::printTo(ostream& os){
  os << "( INTLIT: " << int_literal << " )";
}
float IntLitNode::interpret()
{
  return int_literal;
}
//-------------------------------------------------------------
FloatLitNode::FloatLitNode(int level, float value){
    _level = level - 1;
    float_literal = value;
}
FloatLitNode::~FloatLitNode(){
    if(printDelete)
        cout << "Deleting FactorNode:FloatLitNode" << endl;
}
void FloatLitNode::printTo(ostream& os){
    os << "( FLOATLIT: " << float_literal << " )";
}
float FloatLitNode::interpret()
{
  return float_literal;
}
//-------------------------------------------------------------
NestedExprNode::NestedExprNode(int level, ExpressionNode* en){
    _level = level - 1;
    exprPtr = en;
}
void NestedExprNode::printTo(ostream& os) {
    os << "(" << *exprPtr << ")";
}
NestedExprNode::~NestedExprNode() {
    if(printDelete)
      cout << "Deleting FactorNode:NestedExprNode " << endl;
    delete exprPtr;
    exprPtr = nullptr; 
}
float NestedExprNode::interpret()
{
  return exprPtr->interpret();
}
//--------------------------------------------------------------
MinusNode::MinusNode(int level, FactorNode* factor) {
    _level = level - 1;
    factPtr = factor;
}
void MinusNode::printTo(ostream& os) {
    os << "(-" << *factPtr << " ) ";
}
MinusNode::~MinusNode() {
    if(printDelete)
      cout << "Deleting FactorNode:MinusNode" << endl;
    delete factPtr;
    factPtr = nullptr;
}
float MinusNode::interpret()
{
  return - factPtr->interpret();
}
//-------------------------------------------------------------
NotNode::NotNode(int level, FactorNode* factor) {
    _level = level - 1;
    factPtr = factor;
}
void NotNode::printTo(ostream& os) {
    if(printDelete)
      os << "(NOT " << *factPtr << " ) ";
}
NotNode::~NotNode() {
    if(printDelete)
      cout << "Deleting FactorNode:NotNode" << endl;
    delete factPtr;
    factPtr = nullptr;
}
float NotNode::interpret()
{
  if(truth(factPtr->interpret()))
    return 0.0;
  else 
    return 1.0;
  
}
//---------------------------------------------------------------
TermNode::TermNode(int level, FactorNode* secondFactor) {
  _level = level - 1;
  firstFactor = secondFactor;
}
ostream& operator<<(ostream& os, TermNode& tn) {
  os << endl; indent(tn._level); os << "(term ";
  os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = tn.restFactorOps[i];
    if (op == TOK_MULTIPLY) {
      os << endl; indent(tn._level); os << "* ";
    } else if (op == TOK_DIVIDE) {
      os << endl; indent(tn._level); os << "/ ";
    } else if (op == TOK_AND) {
      os << endl; indent(tn._level); os << "AND ";
    }
	    os << *(tn.restFactors[i]);
	}
  os << endl; indent(tn._level); os << "term) ";
	return os;
}
TermNode::~TermNode() {
  if(printDelete)
    cout << "Deleting TermNode " << endl;
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}
float TermNode::interpret()
{
   // a Term is a Factor followed by 0 or more Factors separated by * or / operations

  // get the value of the first Factor
  float returnValue = firstFactor->interpret();

  int length = restFactorOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Factor
    float nextValue = restFactors[i]->interpret();
    
    // perform the operation (* or /) that separates the Factors
    switch(restFactorOps[i]) {
      case TOK_MULTIPLY:
        returnValue = returnValue * nextValue;
      break;
      case TOK_DIVIDE:
        returnValue = returnValue / nextValue;
      break;
    }
  }
  return returnValue;
}
//----------------------------------------------------------------
Simple_expressionNode::Simple_expressionNode(int level,TermNode* secondTerm) {
  _level = level - 1;
  firstTerm = secondTerm;
}
ostream& operator<<(ostream& os, Simple_expressionNode& sn) {
  os << endl; indent(sn._level); os << "(simple_exp ";
	os << *(sn.firstTerm);

	int length = sn.restTermOps.size();
	for (int i = 0; i < length; ++i) {
		int indentop = sn.restTermOps[i];
    if (indentop == TOK_PLUS) {
      os << endl; indent(sn._level); os << "+ ";
    } else if (indentop == TOK_MINUS) {
      os << endl; indent(sn._level); os << "- ";
    } else if (indentop == TOK_OR) {
      os << endl; indent(sn._level); os << "OR";
    }
		os << *(sn.restTerms[i]);
	}
  os << endl; indent(sn._level); os << "simple_exp) ";
	return os;
}
Simple_expressionNode::~Simple_expressionNode() {
  if(printDelete)
    cout << "Deleting SimpleExpNode " << endl;
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
}
float Simple_expressionNode::interpret()
{
  // get the value of the first Factor
  float returnValue = firstTerm->interpret();

  int length = restTermOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Factor
    float nextValue = restTerms[i]->interpret();
    
    // perform the operation (* or /) that separates the Factors
    switch(restTermOps[i]) {
      case TOK_PLUS:
        returnValue = returnValue + nextValue;
      break;
      case TOK_MINUS:
        returnValue = returnValue - nextValue;
      break;
    }
  }
  return returnValue;
}
//----------------------------------------------------------------
ExpressionNode::ExpressionNode(int level, Simple_expressionNode* fn) {
  _level = level - 1;
  firstSimpleExpr = fn;
}
ostream& operator<<(ostream& os, ExpressionNode& en) 
{
  os << endl; indent(en._level); os << "(expression ";
	os << *(en.firstSimpleExpr);

  if (en.secondSimpleExpr != nullptr)
  {
    os << endl; indent(en._level);
    switch(en.simpleExprOps){
      case TOK_EQUALTO: os << "= "; break;
      case TOK_LESSTHAN: os << "< "; break;
      case TOK_GREATERTHAN: os << "> "; break;
      case TOK_NOTEQUALTO: os << "<> "; break;
    } 
    os << *(en.secondSimpleExpr);
  }
	
  os << endl; indent(en._level); os << "expression) ";
	return os;
}
ExpressionNode::~ExpressionNode() {
  if(printDelete)
    cout << "Deleting ExpressionNode " << endl;
	delete firstSimpleExpr;
  firstSimpleExpr = nullptr;
  delete secondSimpleExpr;
  secondSimpleExpr = nullptr;
}
float ExpressionNode::interpret(){
 
  float fValue = firstSimpleExpr->interpret();

  if(secondSimpleExpr == nullptr) {
    return fValue;
  }

  float sValue = secondSimpleExpr->interpret();

  switch(simpleExprOps) {
    case TOK_EQUALTO:
      if(abs(fValue - sValue) <= EPSILON)
        return 1.0; //T
      else
        return 0.0; //F
      break;
    
    case TOK_LESSTHAN:
      if((sValue - fValue) > EPSILON)
        return 1.0; //T
      else
        return 0.0; //F
      break;

      case TOK_GREATERTHAN:
      if((fValue - sValue) > EPSILON)
        return 1.0; //T
      else
        return 0.0; //F
      break;
    
    case TOK_NOTEQUALTO:
      if(abs(fValue - sValue) > EPSILON)
        return 1.0; //T
      else
        return 0.0; //F
      break;
    }
    return 0.0;
}
//---------------------------------------------------------------
ProgramNode::ProgramNode(int level, BlockNode* firstBlock){
    _level = level - 1;
    block = firstBlock;
}
ostream& operator<<(ostream& os, ProgramNode& pn) {
    os << endl; indent(pn._level);
    os << "(program ";
    os << *(pn.block);
    os << endl; indent(pn._level); 
    os << "program) ";
	  return os;
}
ProgramNode::~ProgramNode(){
    if(printDelete)
      cout << "Deleting ProgramNode" << endl;
    delete block;
    block = nullptr;
}
void ProgramNode::interpret()
{
  block->interpret();
}
//---------------------------------------------------------------
BlockNode::BlockNode(int level, CompoundNode* compound){
    _level = level - 1;
    _compound = compound;
}
ostream& operator<<(ostream& os, BlockNode& bn) {
    os << endl; indent(bn._level);
    os << "(block ";
    os << *(bn._compound);
    os << endl; indent(bn._level); 
    os << "block) ";
	  return os;
}
BlockNode::~BlockNode(){
    if(printDelete)
      cout << "Deleting BlockNode" << endl;
    delete _compound;
    _compound = nullptr;
}
void BlockNode::interpret()
{
  _compound->interpret();
}
//---------------------------------------------------------------
StatementNode::~StatementNode() {}

ostream& operator<<(ostream& os, StatementNode& sn){
    sn.printTo(os);
    return os;
}
void StatementNode::interpret()
{
  this->interpret();
}
//----------------------------------------------------------------
AssignmentNode::AssignmentNode(int level){
  _level = level - 1;
}
void AssignmentNode::printTo(ostream& os){
  os << endl; indent(_level); 
  os << "(assignment_stmt "; 
  os << "( " << ident << " := )";
  os << *expression;
  os << endl; indent(_level); 
  os << "assignment_stmt)";
}
AssignmentNode::~AssignmentNode(){
  if(printDelete)
    cout << "Deleting StatementNode:AssignmentStmtNode" << endl;
  delete expression;
  expression = nullptr;
}
void AssignmentNode::interpret()
{
  symbolTableT::iterator variable = symbolTable.find(ident);

  variable->second = expression->interpret();
}
//----------------------------------------------------------------
IfNode::IfNode(int level){
  _level = level - 1;
}
void IfNode::printTo(ostream& os){
  os << endl; indent(_level);os << "(if_stmt ";
  os << *expression;
  os << endl; indent(_level);os << "(then ";
  os << *then_stmt;
  os << endl; indent(_level);os << "then) ";
  if(else_stmt != nullptr)
  {
  os << endl; indent(_level);os << "(else ";
  os << *else_stmt;
  os << endl; indent(_level);os << "else) ";
  }
  os << endl; indent(_level);os << "if_stmt)";
}
IfNode::~IfNode(){
  if(printDelete)
    cout << "Deleting StatementNode:IfStmtNode" << endl;
  delete expression;
  expression = nullptr;
  delete then_stmt; 
  then_stmt = nullptr;
  delete else_stmt;
  else_stmt = nullptr;
}
void IfNode::interpret()
{
  float returnValue = expression->interpret();

  if (truth(returnValue)){
    then_stmt->interpret();
  } else {
    if(else_stmt != nullptr) {
      else_stmt->interpret();
    }
  }

}
//----------------------------------------------------------------
WhileNode::WhileNode(int level){
  _level = level - 1;
}
void WhileNode::printTo(ostream& os){
  os << endl; indent(_level); os << "(while_stmt ";
  os << *expression;
  os << *statement;
  os << endl; indent(_level); os << "while_stmt)";
}
WhileNode::~WhileNode(){
  if(printDelete)
    cout << "Deleting StatementNode:WhileStmtNode" << endl;
  delete expression; expression = nullptr;
  delete statement; statement = nullptr;
}
void WhileNode::interpret(){
  while (truth(expression->interpret())) 
  {
    statement->interpret();
  }
}
//----------------------------------------------------------------
ReadNode::ReadNode(int level){
  _level = level - 1;
}
void ReadNode::printTo(ostream& os){
  os << endl; indent(_level); 
  os << "(read_stmt ";
  if (!READ.empty()){
    os << "( " << READ << " )";
  }
  os << endl; indent(_level);
  os << "read_stmt)";
}
ReadNode::~ReadNode(){
  if(printDelete)
    cout << "Deleting StatementNode:ReadStmtNode" << endl;
}
void ReadNode::interpret()
{
  
  symbolTableT::iterator variable = symbolTable.find(READ);
  cin >> variable->second; 
 
}
//-----------------------------------------------------------------
WriteNode::WriteNode(int level){
  _level = level - 1;
}
void WriteNode::printTo(ostream& os){
  os << endl; indent(_level); 
  os << "(write_stmt ";
  if(!content.empty()){
    os << "( " << content << " )";
  }
  os << endl; indent(_level);
  os << "write_stmt)";
}
WriteNode::~WriteNode(){
  if(printDelete)
    cout << "Deleting StatementNode:WriteStmtNode" << endl;
}
void WriteNode::interpret()
{
  switch(type){
    case TOK_STRINGLIT:
      cout << content.substr(1, content.length()-2) << endl;
    break;
    case TOK_IDENT:
      symbolTableT::iterator variable = symbolTable.find(content);
      cout << variable->second << endl;
      break;
  }
}
//----------------------------------------------------------------
CompoundNode::CompoundNode(int level){
  _level = level - 1;
}
void CompoundNode::printTo(ostream& os){
  os << endl; indent(_level);
  os << "(compound_stmt ";

  int length = Statements.size();
  for (int i = 0; i < length; i++){
    os << " ";
    Statements[i]->printTo(os);

  }
    os << endl; indent(_level); 
    os << "compound_stmt)";
}
CompoundNode::~CompoundNode(){
  if(printDelete)
    cout << "Deleting StatementNode:CompoundStmtNode" << endl;

  int length = Statements.size();
  for(int i = 0; i < length; ++i){
    delete Statements[i];
    Statements[i] = nullptr;
  }
}
void CompoundNode::interpret(){
  for (StatementNode* stmt : Statements){
    stmt->interpret();
  }
}
