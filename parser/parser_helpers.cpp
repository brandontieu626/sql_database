#include "parser_helpers.h"
bool fail_or_success(int table[][COLUMNS],int current_state){
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


void mark_cells(int table[][COLUMNS],int state,int a,int b,int next_state){
    /**
       * @brief DEBUG
       * function:Mark cells
       * purpose:Mark cells from on state from a to b with next state
       * parameters:int table[][COLUMNS],int state,int a,int b,int next_state
       */

    //Loop from a to b
    for(int col=a;col<=b;col++){
        //Set next state
        table[state][col]=next_state;
    }
}



void mark_success(int table[][COLUMNS],int state){
    /**
       * @brief DEBUG
       * function:Mark Success
       * purpose:Mark state as success
       * parameters:int table[][COLUMNS],int state
       */
    table[state][0]=1;
}
