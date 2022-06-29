/**
 * @file Token.h
 * @author Katarina McGaughy
 * @brief The Token class contains an enum called TokenType which creates types
 * for the tokens to be initialized with. The types consist of various
 * characters that exist in an expression along with a few other types. The
 * Token struct creates a Token object that has a type TokenType and a string.
 * @version 0.1
 * @date 2021-12-06
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <string>
#include <iostream>
#pragma once
using namespace std;

/**
 * @brief TokenType
 * Enum that contains the types of Tokens that will be created from the
 * istream
 */
enum TokenType
{
   binop,
   variable,
   number,
   lparen,
   rparen,
   unknown,
   powop,
   assignop,
   invalid,
   ending,
   eol

};

/**
 * @brief Token
 * This struct creates a Token
 *
 */
struct Token
{

   /**
    * @brief Construct a new Token object
    *
    * @param t : TokenType
    * @param v :string of tokens
    */
   Token(TokenType t, string v) : type_(t), value_(v){};

   /**
    * @brief Construct a new Token object
    * default constructor that sets type and value to unknown
    *
    */
   Token() : type_(unknown), value_("unknown") {}

   // token type from enum
   TokenType type_;

   // token value
   string value_;
};