#ifndef TOKEN_H
#define TOKEN_H
#include <string.h>
#include <cstring>
#include <iostream>
using namespace std;

//All the different states
/*
 * GRAMMAR: is the state when a word has an apostrophe or dash
 * CONTRACTION: is the state when it is a successful word
 * DECIMAL: is the state when an integer has a decimal, possibly a double
 * DOUBLE: is the state when it is a successful double
 * DOTDOT:is the successful state with ...
*/
enum TYPE{ALPHA=10,GRAMMAR=11,CONTRACTION=12,
          NUMBER=20,DECIMAL=21,DOUBLE=22,
          PUNC=30,DOTDOT=31,
          SPACE=40,VAL=44,VALU=45,OPE=46,OPREL=50,UNKNOWN=60};

class Token
{
public:
    Token();
    Token(string str, int type); //Set string and type with CTOR

    //Printing Token object
    friend ostream& operator <<(ostream& outs, const Token& t);

    int type();           //Return _type

    string type_string(); //return string of what type of Token
    string token_str();   //return _token

private:
    string _token;       //String for Token
    int _type;           //Type of Token
};

#endif // TOKEN_H
