/**
 * @file TokenStream.cpp
 * @author Katarina McGaughy
 * @brief The TokenStream wraps the istream and converts various characters
 * into specific Tokens.
 * @version 0.1
 * @date 2021-12-06
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <iostream>
#include "TokenStream.h"
#include <cassert>
using namespace std;

/**
 * @brief Construct a new Token Stream object
 * constructor that initializes the istream object and sets numCharsAdvances_
 * to 0
 * @param inputStream : istream object
 */
TokenStream::TokenStream(istream &inputStream) : is_(inputStream),
                                                 numCharsAdvanced_(0)
{
}

/**
 * operator>>
 * this function takes in the input stream and initializes tokens
 * based on the characters from the input stream
 *
 * @param rhs : token
 * @return TokenStream&: istream
 */
TokenStream &TokenStream::operator>>(Token &rhs)
{
   string value;
   char c;

   is_.get(c);
   numCharsAdvanced_ = 1;

   switch (c)
   {
      // when to use breaks in case
   case '.':
      rhs = Token(ending, ".");
      break;
   case 40:
      rhs = Token(lparen, "(");
      break;
   case 41:
      rhs = Token(rparen, ")");
      break;
   case '+':
      rhs = Token(binop, "+");
      break;
   case '-':
      rhs = Token(binop, "-");
      break;
   case '/':
      rhs = Token(binop, "/");
      break;
   case '*':
      rhs = Token(binop, "*");
      break;
   case '^':
      rhs = Token(powop, "^");
      break;
   case ':':
      if (is_.peek() == '=')
      {
         is_.get(c);
         numCharsAdvanced_++;
         rhs = Token(assignop, ":=");
      }
      else
      {
         rhs = Token(invalid, "invalid");
      }
      break;
   default:
      if (isdigit(c))
      {
         value += c;
         while (isdigit(is_.peek()))
         {
            is_.get(c);
            numCharsAdvanced_++;
            value += c;
         }
         rhs = Token(number, value);
      }
      else if (isalpha(c))
      {
         string var;
         // make character lowercase and add to string
         var = tolower(c);
         rhs = Token(variable, var);
      }
      else if (c == '\n')
      {
         rhs = Token(eol, "\n");
      }
      else
      {
         rhs = Token(invalid, "invalid");
      }
   }
   return *this;
}

/**
 * @brief explicit type converter to a bool which is used to test
 * if there is an error in the input stream
 * @return true : returns true if there are no erros
 * @return false : returns false if errors exist
 */
TokenStream::operator bool() const
{
   Token token;
   TokenStream *nc_this = const_cast<TokenStream *>(this);

   nc_this->operator>>(token);

   // Unget however many characters that operator>> advanced.
   for (int i = 0; i < numCharsAdvanced_; ++i)
   {
      nc_this->is_.unget();
   }
   nc_this->numCharsAdvanced_ = 0;

   if (token.type_ == invalid)
      return false;
   else
      return true;
}
