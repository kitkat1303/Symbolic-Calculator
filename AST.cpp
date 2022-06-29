#include "AST.h"
#include "TokenStream.h"
#include <iostream>
#include <string>
#include <stack>
#include <math.h>
using namespace std;

/**
 * @brief Construct a new AST::AST object
 * this constructor takes in a valid postfix vector of tokens
 * and either assigns an AST to a variable or creates an AST
 * the root_ is then set to point to the AST
 *
 * @param postfix : a postfix vector of tokens
 */
AST::AST(vector<Token> &postfix) : root_(nullptr)
{
   constructTree(postfix);
}

/**
 * @brief Construct a new AST object
 * constructs an AST via only a single token, this is used to initialize the
 * map of variables
 * @param t
 */
AST::AST(Token t) : root_(nullptr)
{
   root_ = new Node(t);
}

/**
 * @brief Construct a new AST object
 * copy constructor that calls copyTree()
 *
 * @param ast
 */
AST::AST(const AST &ast) 
{
   root_ = copyTree(ast.root_);
}

/**
 * @brief operator=
 * this is an assignment operator that assigns the ast passed in
 *
 * @param ast : AST that will be assigned
 * @return AST& : new AST
 */
AST &AST::operator=(const AST &ast)
{
   if (&ast != this) //check for self assignment
   {
      clear(root_);
      root_ = copyTree(ast.root_);
   }                          
   return *this;
}

/**
 * @brief Destroy the AST object
 *
 */
AST::~AST()
{
   clear(root_);
}

/**
 * @brief clear
 * this function takes in a root and clears the entire tree
 *
 * @param root
 */
void AST::clear(Node* &root)
{
   if (root == nullptr)
   {
      return;
   }
   clear(root->left);
   clear(root->right);

   delete root;
   root = nullptr;
}

/**
 * @brief copyTree
 * this function takes in the root of the tree to be copied and copies
 * the tree. It then returns the new node of the copy.
 *
 * @param oldRoot : root of tree to be copied
 * @return Node* : new tree root
 */
AST::Node *AST::copyTree(const Node *oldRoot) const
{
   //check if oldRoot is nullptr
   Node *newRoot = new Node(oldRoot->token);
   if (oldRoot->left != nullptr)
   {
      newRoot->left = copyTree(oldRoot->left);
   }
   if (oldRoot->right != nullptr)
   {
      newRoot->right = copyTree(oldRoot->right);
   }
   return newRoot;
}

/**
 * @brief constructTree
 * this function takes in a postfix vector of tokens and creates an AST,
 * setting the root_ member variable to the root
 *
 * @param postfix : vector of tokens in postfix form
 */
void AST::constructTree(vector<Token> &postfix)
{
   stack<Node *> stack;

   for (int i = 0; i < postfix.size(); i++)
   {
      if (isOperator(postfix[i]) || isPower(postfix[i]))
      {

         Node *left = stack.top();
         stack.pop();

         Node *right = stack.top();
         stack.pop();

         // construct a new binary tree whose root is the operator and whose
         // left and right children point to `y` and `x`
         Node *node = new Node(postfix[i], left, right);

         // push the current node into the stack
         stack.push(node);
      }
      else if (isOperand(postfix[i]))
      {
         // if number or variable, push onto stack
         stack.push(new Node(postfix[i]));
      }
   }
   // set root to point to tree
   root_ = stack.top();
}

/**
 * @brief simplify
 * this method takes in the map of variables and creates a copy of the
 * current AST. It then calles fillVariables and traverseAndSimplify on
 * the copy and returns the simplifed copy
 * @param variables: an array that holds the variables that can be stored
 * @return AST : returned simplified AST
 */
AST AST::simplify(map<string, AST> &variables)
{
   // make a copy called newAST
   AST newAST = AST(*this);

   // modify tree to include variable expressions from the map
   newAST.fillVariables(variables);
   // return the simplified tree
   newAST.traverseAndSimplify();
   return newAST;
}

/**
 * @brief fillVariables
 * this functions calls fillVariablesHelper, which is a recursive method
 *
 * @param variables : map of variables with an associated AST
 */
void AST::fillVariables(map<string, AST> &variables)
{
   root_ = fillVariablesHelper(root_, variables);
}

/**
 * @brief traverseAndSimplify
 * this function calls the recursive travserse and simplify helper function
 *
 */
void AST::traverseAndSimplify()
{
   traverseAndSimplifyHelper(root_);
}

/**
 * @brief fillVariablesHelper
 * this functions takes in a map of variables and fills the variables that are
 * in the current AST with the AST that is stored for that variable. The
 * function takes in a root pointer and the map of variables
 *
 * @param root : node pointer
 * @param variables : map of variables
 * @return Node* : node pointer
 */
AST::Node *AST::fillVariablesHelper(Node *&root, map<string, AST> &variables)
{
   if (root == nullptr)
   {
      return root;
   }

   root->left = fillVariablesHelper(root->left, variables);
   root->right = fillVariablesHelper(root->right, variables);

   if (isVariable(root->token))
   {
      // store variable string in var
      string var = root->token.value_;
      // get the root of the ast via the var and copy the variable ast to
      // where the root is of the variable in the AST
      // do i have to check if the root of variable is left and right nullptr?
      root = copyTree(variables[var].root_);
   }
   return root;
}

/**
 * @brief traverseAndSimplifyHelper
 * this function takes in a node pointer and traverses the tree calling calc()
 * wherever an expression can be simplified
 *
 * @param root : node pointer
 */
void AST::traverseAndSimplifyHelper(Node *&root)
{
   if (root == nullptr)
   {
      return;
   }

   traverseAndSimplifyHelper(root->left);
   traverseAndSimplifyHelper(root->right);
   // can only do something if two numbers and operand or else cant do anything
   if (isOperator(root->token) || isPower(root->token))
   {
      if (root->left->token.type_ == number && root->right->token.type_ 
      == number)
      {
         Token newToken = Token(number, calc(root->left->token.value_,
         root->token.value_, root->right->token.value_));
         delete root->right;
         root->right = nullptr;
         delete root->left;
         root->left = nullptr;
         root->token = newToken;
      }
   }
   return;
}

/**
 * @brief calc
 * this function takes in two operands and an operator and performs
 * the correct calculation
 *
 * @param leftOperand : operand that is left child
 * @param binop : operator
 * @param rightOperand : operand that is right child
 * @return string : string represenation of calculation
 */
string AST::calc(string leftOperand, string binop, string rightOperand) const
{
   int right = stoi(leftOperand);
   int left = stoi(rightOperand);
   int solution;
   if (binop == "+")
   {
      solution = left + right;
   }
   else if (binop == "-")
   {
      solution = left - right;
   }
   else if (binop == "*")
   {
      solution = left * right;
   }
   else if (binop == "/")
   {
      solution = left / right;
   }
   else
   {
      solution = pow(left, right);
   }

   return to_string(solution);
}

/**
 * @brief isOperator
 *
 * @param t : token
 * @return true : if the token is an operator
 * @return false : false if the token is not an operator
 */
bool AST::isOperator(Token t) const
{
   if (t.type_ == binop)
   {
      return true;
   }
   else
   {
      return false;
   }
}

/**
 * @brief isVariable
 *
 * @param t : token
 * @return true : if the token is a variable
 * @return false : false if the token is not a variable
 */
bool AST::isVariable(Token t) const
{
   if (t.type_ == variable)
   {
      return true;
   }
   else
   {
      return false;
   }
}

/**
 * @brief isOperand
 *
 * @param t : token
 * @return true : if the token is an operand
 * @return false : false if the token is not an operand
 */
bool AST::isOperand(Token t) const
{
   if (t.type_ == number || t.type_ == variable)
   {
      return true;
   }
   else
   {
      return false;
   }
}

/**
 * @brief isPower
 *
 * @param t : token
 * @return true : if the token is a power operator
 * @return false : false if the token is not a power operator
 */
bool AST::isPower(Token t) const
{
   if (t.type_ == powop)
   {
      return true;
   }
   else
   {
      return false;
   }
}

/**
 * @brief
 * method calles toInfixHelper, which returns a string of the expression
 * in infix form
 *
 * @return string : infix form of expression
 */
string AST::toInfix(AST &newAST) const
{
   return toInfixHelper(newAST.root_);
}

/**
 * @brief toInfixHelper
 * this function takes in a node pointer and recursively converts the
 * AST into a string infix expression
 *
 * @param node
 * @return string
 */
string AST::toInfixHelper(Node *node) const
{
   string infix;
   if (node == nullptr)
      return infix;

   if (isOperator(node->token))
   {
      string left = toInfixHelper(node->left);
      string right = toInfixHelper(node->right);
      infix += "(";
      infix += left;
      infix += node->token.value_;
      infix += right;
      infix += ")";
   }
   else
   {
      return node->token.value_;
   }

   return infix;
}

AST::Node::Node() : token(unknown, "unknown"), left(nullptr), right(nullptr){}

/**
 * @brief Construct a new Node object
 *
 * @param t : node is made up of a single token
 */
AST::Node::Node(Token t) : token(t), left(nullptr), right(nullptr)
{
}

/**
 * @brief Construct a new Node object
 * default constructor that initializes the pointers
 * @param t : token
 * @param leftptr : left node pointer
 * @param rightptr : right node pointer
 */
AST::Node::Node(Token t, Node *leftptr, Node *rightptr) : token(t),
left(leftptr), right(rightptr) {}
