/**
 * @file AST.h
 * @author Katarina McGaughy
 * @brief The AST class takes in a postfix vector of tokens and constructs
 * an AST. It then simplifies the tree by performing calculations which can be
 * output in infix form.
 * @version 0.1
 * @date 2021-12-06
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <iostream>
#include <vector>
#include <map>
#include "Token.h"
#include "TokenStream.h"
#pragma once

class AST
{

private:
  struct Node
  {
    Node();

    /**
     * @brief Construct a new Node object
     *
     * @param t : node is made up of a single token
     */
    Node(Token t);

    /**
     * @brief Construct a new Node object
     * default constructor that initializes the pointers
     * @param t : token
     * @param leftptr : left node pointer
     * @param rightptr : right node pointer
     */
    Node(Token t, Node *leftptr, Node *rightptr);

    // token stores the type of token it is
    Token token;
    // pointer to left node
    Node *left;
    // pointer to right node
    Node *right;
  };

  // root of the node (top)
  Node *root_;

  /**
   * @brief copyTree
   * this function takes in the root of the tree to be copied and copies
   * the tree. It then returns the new node of the copy.
   *
   * @param oldRoot : root of tree to be copied
   * @return Node* : new tree root
   */
  Node *copyTree(const Node *oldRoot) const;

  /**
   * @brief clear
   * this function takes in a root and clears the entire tree
   *
   * @param root
   */
  void clear(Node* &root);

  /**
   * @brief constructTree
   * this function takes in a postfix vector of tokens and creates an AST,
   * setting the root_ member variable to the root
   *
   * @param postfix : vector of tokens in postfix form
   */
  void constructTree(vector<Token> &postfix);

  /**
   * @brief isOperator
   *
   * @param t : token
   * @return true : if the token is an operator
   * @return false : false if the token is not an operator
   */
  bool isOperator(Token t) const;

  /**
   * @brief isOperand
   *
   * @param t : token
   * @return true : if the token is an operand
   * @return false : false if the token is not an operand
   */
  bool isOperand(Token t) const;

  /**
   * @brief isVariable
   *
   * @param t : token
   * @return true : if the token is a variable
   * @return false : false if the token is not a variable
   */
  bool isVariable(Token t) const;

  /**
   * @brief isPower
   *
   * @param t : token
   * @return true : if the token is a power operator
   * @return false : false if the token is not a power operator
   */
  bool isPower(Token t) const;

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
  string calc(string leftOperand, string binop, string rightOperand) const;

  /**
   * @brief fillVariables
   * this functions calls fillVariablesHelper, which is a recursive method
   *
   * @param variables : map of variables with an associated AST
   */
  void fillVariables(map<string, AST> &variables);

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
  Node *fillVariablesHelper(Node *&root, map<string, AST> &variables);

  /**
   * @brief traverseAndSimplify
   * this function calls the recursive travserse and simplify helper function
   *
   */
  void traverseAndSimplify();

  /**
   * @brief traverseAndSimplifyHelper
   * this function takes in a node pointer and traverses the tree calling calc()
   * wherever an expression can be simplified
   *
   * @param root : node pointer
   */
  void traverseAndSimplifyHelper(Node *&root);

  /**
   * @brief toInfixHelper
   * this function takes in a node pointer and recursively converts the
   * AST into a string infix expression
   *
   * @param node
   * @return string
   */
  string toInfixHelper(Node *node) const;

public:
  /**
   * @brief Construct a new AST object
   * default constructor
   */
  AST() : root_(nullptr) {}

  /**
   * @brief Construct a new AST object
   * constructs an AST via only a single token, this is used to initialize the
   * map of variables
   * @param t
   */
  AST(Token t);

  /**
   * @brief Construct a new AST object
   * this functions takes in a vector of tokens in post fix
   * and creates a binary tree from the nodes up
   *
   * @param postfixExpr
   */
  AST(vector<Token> &postfix);

  /**
   * @brief Construct a new AST object
   * copy constructor that calls copyTree()
   *
   * @param ast
   */
  AST(const AST &ast);

  /**
   * @brief operator=
   * this is an assignment operator that assigns the ast passed in
   *
   * @param ast : AST that will be assigned
   * @return AST& : new AST
   */
  AST &operator=(const AST &ast);

  /**
   * @brief Destroy the AST object
   *
   */
  ~AST();

  /**
   * @brief simplify
   * this method takes in the map of variables and creates a copy of the
   * current AST. It then calles fillVariables and traverseAndSimplify on
   * the copy and returns the simplifed copy
   * @param variables: an array that holds the variables that can be stored
   * @return AST : returned simplified AST
   */
  AST simplify(map<string, AST> &variables);

  /**
   * @brief
   * method calles toInfixHelper, which returns a string of the expression
   * in infix form
   *
   * @return string : infix form of expression
   */
  string toInfix(AST &newAST) const;
};
