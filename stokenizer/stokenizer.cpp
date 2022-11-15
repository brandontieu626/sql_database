#include "stokenizer.h"

STokenizer::STokenizer():_pos(0),_buffer_length(0){
    /**
       * @brief DEBUG
       * function: Default CTOR for STokenizer
       * purpose: Create _table
       * _pos=0
       * _buffer_length=0;
       * parameters: none
       */

    make_table(_table);

}

STokenizer::STokenizer(string str):_pos(0){
    /**
       * @brief DEBUG
       * function: String Buffer CTOR for STokenizer
       * purpose: Create _table
       * Set _str_buffer=str;
       * parameters:string str
       */

    make_table(_table);

    _str_buffer=str;

    _buffer_length=_str_buffer.length(); //Set buffer length

}

STokenizer::STokenizer(char str[]):_pos(0){
    /**
       * @brief DEBUG
       * function: CString Buffer CTOR for STokenizer
       * purpose: Create _table
       * set _str_buffer=str
       * parameters: char str[]
       */

    make_table(_table);

    _str_buffer=str;                 //Converting CString to String

    _buffer_length=_str_buffer.length();  //Set buffer length
}

bool STokenizer::done(){
    /**
       * @brief DEBUG
       * function: Done
       * purpose: Return true if no more characters
       * Return false otherwise
       * parameters:none
       */

    //Return true if _pos is greater than _buffer_length,go to NULL once
    if(_pos>_buffer_length){
        return true;
    }
    else{
        return false;
    }
}

bool STokenizer::more(){
    /**
       * @brief DEBUG
       * function: More
       * purpose: Return false if there are no more
       * Return true if there is more
       * parameters:none
       */

    //Return false if _pos is greater than _buffer_length, go to NULL once
    if(_pos>_buffer_length){
        return false;
    }
    else{
        return true;

    }
}

STokenizer& operator >>(STokenizer& s,Token& t){
    /**
       * @brief DEBUG
       * function: STokenizer ostream operator
       * purpose: Call get_token
       * Give get_token start_state=0
       * Pass it a token& parameter
       * Return s
       * parameters:STokenizer& s,Token& t
       */

    s.get_token(0,t);

    return s;


}

void STokenizer::set_string(string str){
    /**
       * @brief DEBUG
       * function:Set string with string
       * purpose:Reset buffer
       * Set _str_buffer=str
       * _pos=0
       * Get buffer length again
       * parameters:string str
       */

    _str_buffer=str;  //Set _str_buffer to new string

    _pos=0;           //Reset _pos=0

    _buffer_length=_str_buffer.length(); //Set _buffer_length
}

void STokenizer::set_string(char str[]){
    /**
       * @brief DEBUG
       * function:Set string with CString
       * purpose:Reset buffer
       * set _str_buffer=str
       * _pos=0
       * Get buffer length again
       * parameters:char str[]
       */

     _str_buffer=str;  //Set _str_buffer to new string

    _pos=0;            //Reset _pos=0

     _buffer_length=_str_buffer.length(); //Set _buffer_length

}


bool STokenizer::get_token(int start_state,Token& token){
    /**
       * @brief DEBUG
       * function:Get Token
       * purpose: Grab a single token from the _str_buffer
       * Return true if any token was grabbed
       * Return false if no token was found
       * parameters:int start_state, Token& token
       */

    //Last successful position
    int last_success_pos=_pos;

    //Last successful state, starts UNKNOWN
    int last_successful_state=UNKNOWN;

    //Use the start state to go to the next state
    int next_state=start_state;

    char each_char;          //Each extracted character

    string successful_token; //All successful tokens
    string valid_token;      //All valid tokens



    //Check NULL
    if(_str_buffer[_pos]==NULL){

        _pos++;            //Advance position, return false

        return false;
    }
    //Check Negative/Foreign characters
    else if(_str_buffer[_pos]<0){

        _pos++;         //Advance position

        each_char=_str_buffer[_pos];  //Get character

        successful_token+=each_char; //Add it to the string

        //Set token with unknown character and UNKNOWN type
        token=Token(successful_token,UNKNOWN);

        return true;   //Return true
    }

    //Token processing loop
    while(next_state!=-1&&_pos<_buffer_length){

        each_char=_str_buffer[_pos];  //Get Character

        //If the character is negative
        if(each_char<0){
            //Set _pos to one position ahead last successful state
            _pos=last_success_pos+1;

            //Set token to successful token and last successful state
            token=Token(successful_token,last_successful_state);

            //Return true
            return true;
        }

        //Set next_state using ascii table value
        next_state=_table[next_state][each_char];

        //If the next state is not a failing state
        if(next_state!=-1){
            valid_token+=each_char; //Add character to valid token

            //If the next state is successful
            if(fail_or_success(_table,next_state)){
                //Set this as your last successful position
                last_success_pos=_pos;

                //Set successful token=valid token
                successful_token=valid_token;

                //Save this as your last successful state
                last_successful_state=next_state;

            }
        }


        _pos++;  //Advance position
    }

    //Set position to the last success state,
    //Plus 1 since you want to be at the position afterwards
    _pos=last_success_pos+1;

    //If success token is empty, there was no success
    if(successful_token.empty()){
        //Add the unsuccessful character to string
        successful_token+=each_char;

        //Set token to the token and UNKNOWN type
        token=Token(successful_token,UNKNOWN);

        return true;
    }
    //If the token is not empty, there was a success
    else{
        //Set token with successful token and last successful state
        token=Token(successful_token,last_successful_state);


        return true;
    }

}

void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    /**
       * @brief DEBUG
       * function: Make table
       * purpose: Set up the table with all the states
       * Mark success states
       * Mark next states/columns
       * parameters:int _table[][MAX_COLUMNS]
       */

    //Initialize _table to all -1
    init_table(_table);


    //Mark cells for state 0 to lead to corresponding state
    mark_cells(_table,0,'A','Z',ALPHA);

    mark_cells(_table,0,'a','z',ALPHA);

    mark_cells(_table,0,' ',' ',SPACE);

    mark_cells(_table,0,",:;?()[]'!-/*=_",PUNC);

    mark_cells(_table,0,'"','"',VAL);

    mark_cells(_table,0,".",DOTDOT);

    mark_cells(_table,0,'0','9',NUMBER);

    mark_cells(_table,0,'<','<',OPE);

    mark_cells(_table,0,'>','>',OPE);


    //Mark cells with states and next states
    mark_cells(_table,ALPHA,'A','Z',ALPHA);

    mark_cells(_table,ALPHA,'a','z',ALPHA);

    mark_cells(_table,ALPHA,"-'",GRAMMAR);

    mark_cells(_table,GRAMMAR,'A','Z',CONTRACTION);

    mark_cells(_table,GRAMMAR,'a','z',CONTRACTION);

    mark_cells(_table,CONTRACTION,'A','Z',CONTRACTION);

    mark_cells(_table,CONTRACTION,'a','z',CONTRACTION);

    mark_cells(_table,SPACE,' ',' ',SPACE);

    mark_cells(_table,DOTDOT,".",DOTDOT);

    mark_cells(_table,NUMBER,'0','9',NUMBER);

    mark_cells(_table,NUMBER,".",DECIMAL);

    mark_cells(_table,DECIMAL,'0','9',DOUBLE);

    mark_cells(_table,DOUBLE,'0','9',DOUBLE);

    mark_cells(_table,OPE,'=','=',OPREL);

    mark_cells(_table,VAL,'A','Z',VAL);

    mark_cells(_table,VAL,'a','z',VAL);

    mark_cells(_table,VAL,' ',' ',VAL);

    mark_cells(_table,VAL,",:;?()[]'!-/*.",VAL);

    mark_cells(_table,VAL,'0','9',VAL);

    mark_cells(_table,VAL,'"','"',VALU);


    //Success Marking

    mark_success(_table,DOTDOT);

    mark_success(_table,PUNC);

    mark_success(_table,ALPHA);

    mark_success(_table,SPACE);

    mark_success(_table,NUMBER);

    mark_success(_table,CONTRACTION);

    mark_success(_table,DOUBLE);

    mark_success(_table,OPE);

    mark_success(_table,OPREL);

    mark_success(_table,VALU);

}

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];
