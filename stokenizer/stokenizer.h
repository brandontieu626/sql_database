#ifndef STOKENIZER_H
#define STOKENIZER_H
#include "token.h"
#include "table_functions.h"

const int MAX_BUFFER=500;
class STokenizer
{
public:
    STokenizer();
    STokenizer(string str); //String buffer CTOR
    STokenizer(char str[]); //CString buffer CTOR

    bool done();            //true:there are no more tokens
    bool more();            //true:there are more tokens

    //Get one token, call get_token
    friend STokenizer& operator >>(STokenizer& s, Token& t);


    void set_string(string str);  //Set buffer using string
    void set_string(char str[]);  //Set buffer using cstring

private:
    void make_table(int _table[][MAX_COLUMNS]);   //Creating table
    bool get_token(int start_state,Token& token);

    string _str_buffer;              //Buffer

    int _pos;                        //Position in buffer

    int _buffer_length;              //Length of buffer

    static int _table[MAX_ROWS][MAX_COLUMNS];


};

#endif // STOKENIZER_H
