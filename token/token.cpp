#include "token.h"

Token::Token():_token(""),_type(UNKNOWN){
    /**
       * @brief DEBUG
       * function:Token Default CTOR
       * purpose:Set _token to an empty string
       * _type=UNKNOWN
       * parameters:none
       */

    //Empty
}

Token::Token(string str, int type){
    /**
       * @brief DEBUG
       * function:Token CTOR with given string and type
       * purpose:_token=str
       * _type=type
       * parameters:string str, int type
       */
    _token=str;
    _type=type;
}

ostream& operator<<(ostream& outs,const Token& t){
    /**
       * @brief DEBUG
       * function:Ostream operator for Token
       * purpose:Print t._token
       * return ostream& outs
       * parameters:ostream& outs, const Token& t
       */

    outs<<"|"<<t._token<<"|";

    return outs;
}

int Token::type(){
    /**
       * @brief DEBUG
       * function:Type
       * purpose:Return _type
       * parameters:none
       */
    return _type;
}

string Token::type_string(){
    /**
       * @brief DEBUG
       * function:Type _string
       * purpose:Return a string for what type of Token
       * parameters:none
       */
    switch(_type)
    {
    case ALPHA:
    case CONTRACTION:
        return "ALPHA";
    case DOUBLE:
    case NUMBER:
        return "NUMBER";
    case PUNC:
    case DOTDOT:
        return "PUNC";
    case SPACE:
        return "SPACE";
    case UNKNOWN:
        return "UNKNOWN";
    }
}

string Token::token_str(){
    /**
       * @brief DEBUG
       * function:Token Str
       * purpose:Return _token
       * return the string of token
       * parameters:none
       */
    return _token;
}
