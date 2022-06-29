/**
 * @file TokenStream.h
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
#include "Token.h"
#pragma once

class TokenStream
{

private:
   // istream object
   istream &is_;

   // keeps track of how many characters have been called by get()
   int numCharsAdvanced_;

public:
   /**
    * @brief Construct a new Token Stream object
    * constructor that initializes the istream object and sets numCharsAdvances_
    * to 0
    * @param inputStream : istream object
    */
   TokenStream(istream &inputStream);

   /**
    * operator>>
    * this function takes in the input stream and initializes tokens
    * based on the characters from the input stream
    *
    * @param rhs : token
    * @return TokenStream&: istream
    */
   TokenStream &operator>>(Token &rhs);

   /**
    * @brief explicit type converter to a bool which is used to test
    * if there is an error in the input stream
    * @return true : returns true if there are no erros
    * @return false : returns false if errors exist
    */
   explicit operator bool() const;
};
