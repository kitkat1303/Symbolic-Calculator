
#include <iostream>
#include "Calc.h"
#include <vector>
#include "TokenStream.h"
#include "Token.h"
#include "AST.h"
using namespace std;

int main(){

 cout << "Running Calculator Program " << endl;
 cout << "Please input expressions: " << endl;  
 Calc calc = Calc();
 calc.calculate();

   return 0;
}
