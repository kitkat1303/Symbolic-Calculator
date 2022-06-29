/**
 * @file Calc.h
 * @author Katarina McGaughy
 * @brief The Calc class takes in a stream of tokens, ensures the stream of
 * tokens is a valid expression, converts the expression to postfix form and
 * assigns any variables that need to be assigned. It then creates an AST and
 * simplifies the expression via the AST class and outputs the simplified
 * expression in infix form.
 *
 * @version 0.1
 * @date 2021-12-06
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <iostream>
#include <vector>
#pragma once
#include "TokenStream.h"
#include "Token.h"
#include "AST.h"
#include <map>

class Calc
{

public:
   /**
    * @brief Construct a new Calc object
    * initializes istream and the map of variables
    */
   Calc();

   /**
    * @brief Destroy the Calc object
    *
    */
   ~Calc() {}

   /**
    * @brief calculate
    * this function continuously asks the user for input until a . is entered.
    * It stores the tokens from input into a vector and then checks to ensure
    * that the input is a valid infix expression via the isValid() function.
    * If it is not, it will display an error message and continue to get input
    * from user. If it is a valid expression, the expression is checked to see
    * if it is an assignment. If it is an assignment, a new infix vector is
    * formed without the variable and expression operator and then saved in the
    * map of variables. The infix is converted to postfix via the
    * convertPostFix() function. From there, an AST is created by passing
    * in the postfix vector of tokens along with the map that holds
    * the variables and their assignments.
    * The AST is then simplified via the simplify() function and the solution is
    * printed to the screen via the toInfix() function. The user can continue to
    * enter expressions after this until the end condition is met.
    *
    */
   void calculate();

   /**
    * @brief displayInputAndOutput
    * this function takes in two vectors of strings that hold the input and
    * output and print them.
    *
    * @param expressions : expressions that were input to calculator
    * @param solutions : solutions of the expressions
    */
   void displayInputAndOutput(vector<string> &expressions,
   vector<string> &solutions) const;

   /**
    * @brief convertPostfix
    * this function takes in an infix vector of tokens and returns the postfix
    * version
    * PRE: needs to be a valid infix expression
    *
    * @param infix : infix expression vector of tokens
    * @return vector<Token> : postfix vector of tokens
    */
   vector<Token> convertPostfix(const vector<Token> &infix);

   /**
    * @brief isAssignment
    *
    * @param postfix : takes in an infix vector
    * @return true : if the vector is an assignment
    * @return false : if it is not an assignment
    */
   bool isAnAssignment(vector<Token> &infix) const;

   /**
    * @brief assignVariable
    * this function takes in the map of variables and the postfix expression
    * and assigns the AST created from the postfix expression to the variable
    *
    * @param v : string variable
    * @param variables : map of variables
    * @param postfix : postfix vector
    */
   void assignVariable(string v,
                       vector<Token> &postfix);

   /**
    * @brief assignVariableHelper
    * this function creates a new infix vector without variable and assignment
    * operator and then converts the infix vector to postfix. It then calls
    * assignVariable
    *
    * @param infix
    * @param variables
    * @return vector<Token>
    */
   vector<Token> assignVariableHelper(vector<Token> &infix);

   /**
    * @brief isValid
    * this functions checks to ensure that the infix vector of tokens is a valid
    * expression by calling various functions
    *
    * @param infix : infix vector of tokens
    * @return true : if expression is valid
    * @return false : if it is not valid
    */
   bool isValid(vector<Token> &infix) const;

private:
   // stream of tokens
   TokenStream tstream;

   // map of variables that hold an AST
   map<string, AST> variables;

   /**
    * @brief initializeVariables
    * this functions initializes the map of variables to an AST that is just
    * a token with a type of variable and value that is a character from the
    * alphabet
    *
    */
   void initializeVariables();

   /**
    * @brief precedence
    * this functions assigns precedence of operations via a number ranking
    * system
    *
    * @param t : token
    * @return int : precedence rank
    */
   int precedence(Token t) const;

   /**
    * @brief isLeftParen
    *
    * @param t : token
    * @return true : if is a left parenthesis
    * @return false : if not
    */
   bool isLeftParen(Token t) const;

   /**
    * @brief isRightParen
    *
    * @param t : token
    * @return true : if is a right parenthesis
    * @return false : false if not
    */
   bool isRightParen(Token t) const;

   /**
    * @brief hasPrecedence
    * this functions compares precedence ranks of two tokens
    *
    * @param tok1 : first token
    * @param tok2 : second token
    * @return true : returns true if first token has precedence
    * @return false : false if not
    */
   bool hasPrecedence(Token tok1, Token tok2) const;

   /**
    * @brief isRightAssoc
    * this function takes in a token and determines if it is right associative
    *
    * @param t : token
    * @return int : returns true if right associative (^)
    */
   bool isRightAssoc(Token t) const;

   /**
    * @brief isValidToken
    *
    * @param infix
    * @return true : if is a valid token
    * @return false : if not
    */
   bool isValidToken(vector<Token> &infix) const;

   /**
    * @brief isValidFirstToken
    *
    * @param infix : infix vector of tokens
    * @return true : if the first token is a valid type
    * @return false : if not valid
    */
   bool isValidFirstToken(vector<Token> &infix) const;

   /**
    * @brief isValidLastToken
    *
    * @param infix : infix vector of tokens
    * @return true : if the last token is a valid type
    * @return false : if not
    */
   bool isValidLastToken(vector<Token> &infix) const;

   /**
    * @brief isValidSize
    *
    * @param infix : infix vector of tokens
    * @return true : if the size of the vector is less than 80 tokens
    * @return false : if it is larger than 80 tokens
    */
   bool isValidSize(vector<Token> &infix) const;

   /**
    * @brief isValidBinop
    *
    * @param infix : infix vector of tokens
    * @return true : if the token after binop is allowed
    * @return false : if it is not allowed
    */
   bool isValidBinop(vector<Token> &infix) const;

   /**
    * @brief isValidVariable
    *
    * @param infix : infix vector of tokens
    * @return true : if the token after variable is allowed
    * @return false : if it is not allowed
    */
   bool isValidVariable(vector<Token> &infix) const;

   /**
    * @brief isValidNumber
    *
    * @param infix : infix vector of tokens
    * @return true : if token after number is allowed
    * @return false : if it is not allowed
    */
   bool isValidNumber(vector<Token> &infix) const;

   /**
    * @brief isValidPowop
    *
    * @param infix : infix vector of tokens
    * @return true : if the token after the power operator is valid
    * @return false : if it is not valid
    */
   bool isValidPowop(vector<Token> &infix) const;

   bool isValidPowHelper(vector<Token> &infix, int lparen) const;

   /**
    * @brief isValidAssignop
    *
    * @param infix : infix vector of tokens
    * @return true : if the token after assignop is allowed
    * @return false : if it is not allowed
    */
   bool isValidAssignop(vector<Token> &infix) const;

   /**
    * @brief isValidEol
    *
    * @param infix : infix vector of tokens
    * @return true : if the eol is at the end of the expression
    * @return false : if it is not at end of expression
    */
   bool isValidEol(vector<Token> &infix) const;

   /**
    * @brief
    *
    * @param infix : infix vector of tokens
    * @return true : if the token after the rparen is valid
    * @return false if it is not allowed
    */
   bool isValidRParen(vector<Token> &infix) const;

   /**
    * @brief
    *
    * @param infix : infix vector of tokens
    * @return true : if the token after lparen is valid
    * @return false : if it is not allowed
    */
   bool isValidLParen(vector<Token> &infix) const;
};
