/**
 * @file Calc.cpp
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
#include "Calc.h"
#include "AST.h"
#include <iostream>
#include <stack>
#include <vector>
#include <string>
using namespace std;

/**
 * @brief Construct a new Calc object
 * initializes istream and the map of variables
 */
Calc::Calc() : tstream(cin), variables()
{
   initializeVariables();
}

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
void Calc::calculate()
{
   vector<string> expressions;
   vector<string> solutions;

   bool done = false;

   // keep calculating expressions until done
   while (!done)
   {
      Token tok = Token();
      vector<Token> infix;
      TokenStream tstream(cin);

      // while not at end of line
      while (tok.type_ != eol)
      {
         // read in tokens from stream and add to infix vector
         tstream >> tok;
         infix.push_back(tok);
      }

      // if the first token is ending token, end program
      if (infix[0].type_ == ending)
      {
         cout << "Exiting calculator." << endl;
         displayInputAndOutput(expressions, solutions);
         done = true;
      }

      // if infix is a valid expression, convert to postfix vector
      if (isValid(infix))
      {
         string expr;
         for (int i = 0; i < infix.size(); i++)
         {
            expr += infix[i].value_;
         }
         // add expression to vector to print later
         expressions.push_back(expr);

         // if it is an assignment, store expression in variable
         if (isAnAssignment(infix))
         {
            vector<Token> postfix;
            postfix = assignVariableHelper(infix);
            AST ast = AST(postfix);
            // Make a copy of the original AST to simplify.
            AST simplifiedAST = ast.simplify(variables);
            string sol = ast.toInfix(simplifiedAST);
            solutions.push_back(sol);
         }
         else if (infix[0].type_ == variable && infix[1].type_ == eol)
         {
            // add variable again to assignopp and then add assignop to vector
            AST ast = AST(infix);
            Token t = Token(assignop, ":=");
            infix.insert(infix.begin(), t);
            infix.insert(infix.begin(), infix[0]);
            vector<Token> postfix;
            postfix = assignVariableHelper(infix);
            // Make a copy of the original AST to simplify.
            AST simplifiedAST = ast.simplify(variables);
            string sol = ast.toInfix(simplifiedAST);
            solutions.push_back(sol);
         }
         else
         {
            vector<Token> postfix;
            postfix = convertPostfix(infix);
            AST ast = AST(postfix);
            AST simplifiedAST = ast.simplify(variables);
            string sol = ast.toInfix(simplifiedAST);
            solutions.push_back(sol);
         }
      }
   }
}

/**
 * @brief displayInputAndOutput
 * this function takes in two vectors of strings that hold the input and
 * output and print them.
 *
 * @param expressions : expressions that were input to calculator
 * @param solutions : solutions of the expressions
 */
void Calc::displayInputAndOutput(vector<string> &expressions,
                                 vector<string> &solutions) const
{
   cout << "Displaying all input and output below: " << endl;
   for (int i = 0; i < expressions.size(); i++)
   {
      cout << "in  [" << i << "]: ";
      cout << expressions[i];
      cout << "out [" << i << "]: ";
      cout << solutions[i] << endl;
   }
}

/**
 * @brief initializeVariables
 * this functions initializes the map of variables to an AST that is just
 * a token with a type of variable and value that is a character from the
 * alphabet
 *
 */
void Calc::initializeVariables()
{
   string alph = "abcdefghijklmnopqrstuvwxyz";
   for (int i = 0; i < alph.length(); i++)
   {
      string var;
      var = alph[i];
      Token t = Token(variable, var);
      AST ast = AST(t);
      variables.insert(pair<string, AST>(var, ast));
   }
}

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
vector<Token> Calc::assignVariableHelper(vector<Token> &infix)
{
   string variable = infix[0].value_;

   infix.erase(infix.begin());
   infix.erase(infix.begin());
   vector<Token> postfix = convertPostfix(infix);
   // add the variable along with the AST to the map of variables
   assignVariable(variable, postfix);
   return postfix;
}

/**
 * @brief assignVariable
 * this function takes in the map of variables and the postfix expression
 * and assigns the AST created from the postfix expression to the variable
 *
 * @param v : string variable
 * @param variables : map of variables
 * @param postfix : postfix vector
 */
void Calc::assignVariable(string v,
                          vector<Token> &postfix)
{
   if (postfix.size() == 2)
   {
      return;
   }
   // if variable exists, delete and store new AST
   map<string, AST>::iterator it = variables.find(v);
   if (it != variables.end())
   {
      variables.erase(it);
   }
   AST variableAST = AST(postfix);
   // store AST
   variables.insert(pair<string, AST>(v, variableAST));
}

/**
 * @brief isAnAssignment
 *
 * @param postfix : takes in an infix vector
 * @return true : if the vector is an assignment
 * @return false : if it is not an assignment
 */
bool Calc::isAnAssignment(vector<Token> &infix) const
{
   if (infix[0].type_ == variable && infix[1].type_ == assignop)
   {
      return true;
   }
   return false;
}

/**
 * @brief isValid
 * this functions checks to ensure that the infix vector of tokens is a valid
 * expression by calling various functions
 *
 * @param infix : infix vector of tokens
 * @return true : if expression is valid
 * @return false : if it is not valid
 */
bool Calc::isValid(vector<Token> &infix) const
{
   if (isValidToken(infix) && isValidFirstToken(infix) &&
                   isValidLastToken(infix) && isValidSize(infix) &&
                   isValidBinop(infix) &&
                   isValidAssignop(infix) && isValidEol(infix) &&
                   isValidLParen(infix) &&
                   isValidNumber(infix) && isValidPowop(infix) &&
                   isValidRParen(infix)){
                      return true;
                   }

   return false;
}

/**
 * @brief isValidToken
 *
 * @param infix
 * @return true : if is a valid token
 * @return false : if not
 */
bool Calc::isValidToken(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (infix[i].type_ == invalid)
      {
         cout << "Invalid character entered." << endl;
         cout << "Please enter a valid expression." << endl;
         return false;
      }
   }
   return true;
}

/**
 * @brief isValidFirstToken
 *
 * @param infix : infix vector of tokens
 * @return true : if the first token is a valid type
 * @return false : if not valid
 */
bool Calc::isValidFirstToken(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (infix[0].type_ == variable || infix[0].type_ == number ||
          infix[0].type_ == lparen)
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   return true;
}

/**
 * @brief isValidLastToken
 *
 * @param infix : infix vector of tokens
 * @return true : if the last token is a valid type
 * @return false : if not
 */
bool Calc::isValidLastToken(vector<Token> &infix) const
{
   if (infix[infix.size() - 1].type_ == eol)
   {
      return true;
   }
   else
   {
      cout << "Invalid last token." << endl;
      return false;
   }
   return true;
}

/**
 * @brief isValidSize
 *
 * @param infix : infix vector of tokens
 * @return true : if the size of the vector is less than 80 tokens
 * @return false : if it is larger than 80 tokens
 */
bool Calc::isValidSize(vector<Token> &infix) const
{
   if (infix.size() > 80)
   {
      cerr << "Error, input must be no longer than 80 tokens." << endl;
      return false;
   }
   return true;
}

/**
 * @brief isValidBinop
 *
 * @param infix : infix vector of tokens
 * @return true : if the token after binop is allowed
 * @return false : if it is not allowed
 */
bool Calc::isValidBinop(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (i + 1 <= infix.size())
      {
         if (infix[i].type_ == binop)
         {
            if (infix[i + 1].type_ == variable || infix[i + 1].type_ == number || infix[i + 1].type_ == lparen)
            {
               return true;
            }
            else
            {
               cout << "Invalid character after operator." << endl;
               return false;
            }
         }
      }
   }
   return true;
}

/**
 * @brief isValidVariable
 *
 * @param infix : infix vector of tokens
 * @return true : if the token after variable is allowed
 * @return false : if it is not allowed
 */
bool Calc::isValidVariable(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (i + 1 <= infix.size())
      {
         if (infix[i].type_ == variable)
         {
            if (infix[i + 1].type_ == binop || infix[i + 1].type_ == rparen || infix[i + 1].type_ == powop ||
                infix[i + 1].type_ == assignop ||
                infix[i + 1].type_ == eol)
            {
               return true;
            }
            else
            {
               cout << "Invalid character after variable." << endl;
               return false;
            }
         }
      }
   }
   return true;
}

/**
 * @brief isValidNumber
 *
 * @param infix : infix vector of tokens
 * @return true : if token after number is allowed
 * @return false : if it is not allowed
 */
bool Calc::isValidRParen(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (i + 1 <= infix.size())
      {
         if (infix[i].type_ == rparen)
         {
            if (infix[i + 1].type_ == binop || infix[i + 1].type_ == rparen ||
                infix[i + 1].type_ == powop ||
                infix[i + 1].type_ == eol)
            {
               return true;
            }
            else
            {
               cout << "Invalid character after right parenthesis." << endl;
               return false;
            }
         }
      }
   }
   return true;
}

/**
 * @brief isValidPowop
 *
 * @param infix : infix vector of tokens
 * @return true : if the token after the power operator is valid
 * @return false : if it is not valid
 */
bool Calc::isValidNumber(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (i + 1 <= infix.size())
      {
         if (infix[i].type_ == number)
         {
            if (infix[i + 1].type_ == binop || infix[i + 1].type_ == rparen ||
                infix[i + 1].type_ == powop ||
                infix[i + 1].type_ == eol)
            {
               return true;
            }
            else
            {
               cout << "Invalid character after number." << endl;
               return false;
            }
         }
      }
   }
   return true;
}

/**
 * @brief isValidAssignop
 *
 * @param infix : infix vector of tokens
 * @return true : if the token after assignop is allowed
 * @return false : if it is not allowed
 */
bool Calc::isValidPowop(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (i + 1 <= infix.size())
      {
         if (infix[i].type_ == powop)
         {
            if (infix[i + 1].type_ == number)
            {
               return true;
            }
            if (infix[i + 1].type_ == lparen)
            {
               return isValidPowHelper(infix, i+1);
            }
            else
            {
               cout << "Invalid character after power operator." << endl;
               return false;
            }
         }
      }
   }
   return true;
}

bool Calc::isValidPowHelper(vector<Token> &infix, int lparen) const
{
   int rparen;
   for (int j = lparen + 1; j <= infix.size(); j++)
   {
      if (infix[j].type_ == rparen)
      {
         j = rparen;
      }
   }
   for (int j = lparen + 1; j <= rparen; j++)
   {
      if (infix[j].type_ == variable)
      {
         cout << "Invalid character after power operator." << endl;
         return false;
      }
   }
   return true;
}

/**
 * @brief isValidEol
 *
 * @param infix : infix vector of tokens
 * @return true : if the eol is at the end of the expression
 * @return false : if it is not at end of expression
 */
bool Calc::isValidAssignop(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (i + 1 < infix.size())
      {
         if (infix[i].type_ == assignop)
         {
            if (infix[i + 1].type_ == number || infix[i + 1].type_ == lparen ||
                infix[i + 1].type_ == variable)
            {
               return true;
            }
            else
            {
               cout << "Invalid character after assignment operator." << endl;
               return false;
            }
         }
      }
   }
   return true;
}

/**
 * @brief
 *
 * @param infix : infix vector of tokens
 * @return true : if the token after the rparen is valid
 * @return false if it is not allowed
 */
bool Calc::isValidEol(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (infix[i].type_ == eol)
      {
         if (i == infix.size() - 1)
         {
            return true;
         }
         else
         {
            cout << "Invalid character after power operator." << endl;
         }
      }
   }
   return true;
}

/**
 * @brief
 *
 * @param infix : infix vector of tokens
 * @return true : if the token after lparen is valid
 * @return false : if it is not allowed
 */
bool Calc::isValidLParen(vector<Token> &infix) const
{
   for (int i = 0; i < infix.size(); i++)
   {
      if (i + 1 <= infix.size())
      {
         if (infix[i].type_ == lparen)
         {
            if (infix[i + 1].type_ == number || infix[i + 1].type_ == lparen ||
                infix[i + 1].type_ == variable)
            {
               return true;
            }
            else
            {
               cout << "Invalid character after left parenthesis." << endl;
               return false;
            }
         }
      }
   }
   return true;
}

/**
 * @brief precedence
 * this functions assigns precedence of operations via a number ranking
 * system
 *
 * @param t : token
 * @return int : precedence rank
 */
int Calc::precedence(Token t) const
{
   if (t.type_ == powop)
   {
      return 3;
   }
   else if (t.value_ == "/" || t.value_ == "*")
   {
      return 2;
   }
   else if (t.value_ == "+" || t.value_ == "-")
   {
      return 1;
   }
   else
   {
      return -1;
   }
}

/**
 * @brief isLeftParen
 *
 * @param t : token
 * @return true : if is a left parenthesis
 * @return false : if not
 */
bool Calc::isLeftParen(Token t) const
{
   if (t.type_ == lparen)
   {
      return true;
   }
   return false;
}

/**
 * @brief isRightParen
 *
 * @param t : token
 * @return true : if is a right parenthesis
 * @return false : false if not
 */
bool Calc::isRightParen(Token t) const
{
   if (t.type_ == rparen)
   {
      return true;
   }
   return false;
}

/**
 * @brief hasPrecedence
 * this functions compares precedence ranks of two tokens
 *
 * @param tok1 : first token
 * @param tok2 : second token
 * @return true : returns true if first token has precedence
 * @return false : false if not
 */
bool Calc::hasPrecedence(Token tok1, Token tok2) const
{
   int t1 = precedence(tok1);
   int t2 = precedence(tok2);
   if (t1 == t2)
   {
      if (isRightAssoc(tok1))
      {
         return false;
      }
      return true;
   }
   return t1 > t2;
}

/**
 * @brief isRightAssoc
 * this function takes in a token and determines if it is right associative
 *
 * @param t : token
 * @return int : returns true if right associative (^)
 */
bool Calc::isRightAssoc(Token t) const
{
   if (t.type_ == powop)
      return true;
   return false;
}

/**
 * @brief convertPostfix
 * this function takes in an infix vector of tokens and returns the postfix
 * version
 * PRE: needs to be a valid infix expression
 *
 * @param infix : infix expression vector of tokens
 * @return vector<Token> : postfix vector of tokens
 */
vector<Token> Calc::convertPostfix(const vector<Token> &infix)
{

   vector<Token> postfix;

   stack<Token> s;

   for (int i = 0; i < infix.size(); i++)
   {

      Token t = infix[i];

      if (infix[i].type_ == number || infix[i].type_ == variable)
      {
         postfix.push_back(infix[i]);
      }

      else if (infix[i].type_ == binop || infix[i].type_ == powop)
      {
         while (!s.empty() && !isLeftParen(s.top()) &&
                hasPrecedence(s.top(), infix[i]))
         {
            postfix.push_back(s.top());
            s.pop();
         }
         s.push(infix[i]);
      }
      else if (isLeftParen(infix[i]))
      {
         s.push(infix[i]);
      }
      else if (isRightParen(infix[i]))
      {
         while (!s.empty() && !isLeftParen(s.top()))
         {
            postfix.push_back(s.top());
            s.pop();
         }
         s.pop();
      }
   }

   // pop and append all operators till Stack is not empty
   while (!s.empty())
   {
      postfix.push_back(s.top());
      s.pop();
   }

   return postfix;
}
