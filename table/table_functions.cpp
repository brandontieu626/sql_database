#include "table_functions.h"

void init_table(int table[][MAX_COLUMNS]){
    /**
       * @brief DEBUG
       * function:Init table
       * purpose: Initialize entire table to all -1
       * parameters:
       */

    //Initialize Row
    for(int i=0;i<MAX_ROWS;i++){
        //Initialize Columns
        for(int j=0;j<MAX_COLUMNS;j++){
            table[i][j]=-1;
        }
    }

}

bool fail_or_success(int table[][MAX_COLUMNS],int current_state){
    /**
       * @brief DEBUG
       * function: Fail or success
       * purpose:Return true if a success
       * Return false is otherwise
       * parameters:int table[][MAX_COLUMNS], int current state
       */

    //If the state's column 0 is ==1, it is a success
    if(table[current_state][0]==1){
        return true;
    }
    //Otherwise return false
    else{
        return false;
    }

}


void mark_cells(int table[][MAX_COLUMNS],int state,char a, char b,int next_state){
    /**
       * @brief DEBUG
       * function: Mark Cells
       * purpose:Mark cells from a to b, on this state with next_state
       * parameters:int table[][MAX_COLUMNS],int state, char a ,char b, int next_state
       */

    //Iterate from column a to column b, on state, mark these columns with next_state
    for(char col=a;col<=b;col++){
        table[state][col]=next_state;
    }
}

void mark_cells(int table[][MAX_COLUMNS],int state,string characters,int next_state){
    /**
       * @brief DEBUG
       * function:Mark Cells
       * purpose:Mark all the characters in string characters on this state to with next_state
       * parameters:int table[][MAX_COLUMNS],int state,string characters,int next_state
       */

    //Length of characters
    int length=characters.length();

    //Position starts at 0
    int position=0;

    //Each character in the string
    char each_char;

    //Iteratre through string
    while(position<length){
        each_char=characters[position];  //Get character

        table[state][each_char]=next_state; //Set it to next state

        position++;                       //Advance position
    }


}


void mark_success(int table[][MAX_COLUMNS],int state){
    /**
       * @brief DEBUG
       * function:Mark Success
       * purpose:Mark state as a success
       * set [state][0]=1
       * parameters:int table[][MAX_COLUMNS], int state
       */
    table[state][0]=1;

}
