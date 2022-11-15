#include "Shunting_yard.h"

Shunting_yard::Shunting_yard(){
    /**
       * @brief DEBUG
       * function:Shunting yard default CTOR
       * purpose:Set up operators map
       * Set up precedence map
       * parameters:none
       */

    //Set up operators
    set_operators();

    //Set up precedence
    set_precedence();
}

Vector<string> Shunting_yard::to_postfix(Vector<string> infix){
    /**
       * @brief DEBUG
       * function:To Postfix
       * purpose:Return a postfix version of the infix
       * parameters:Vector<string> infix
       */
    MyStack<string> op_stack; //stack for operator
    Vector<string> postfix;   //Vector for postfix

    //Loop until the infix is empty
    while(!infix.empty()){
        //Grab type to see what type of token is the front
        //of the Queue/Vector
        switch(grab_type(infix[0])){
        case SYMBOL:
            //If symbol, add to postfix Vector
            postfix+=infix[0];

            //Pop the front of infix
            infix.erase(0);
            break;
        case OP:
        {
            //Grab the Operator
            string q_oper=infix[0];

            //Check if the stack is empty
            if(op_stack.empty()){
                //Push Operator into stack and pop front of infix
                op_stack.push(q_oper);
                infix.erase(0);
            }
            //If top of stack is the left parenthesis
            else if(grab_type(op_stack.top())==LPAR){
                //Put yourself on top of him
                op_stack.push(q_oper);

                //Pop front of infix
                infix.erase(0);
            }
            //Check Precendence
            else{
                //Find precendence using the map
                //Front of Queue's Operator Precendence
                int q_precendence=precendence[q_oper];

                //Find top of Stack's Operator Precendence
                int stack_precendence=precendence[op_stack.top()];

                //If the Q Precendence is <= to Stack
                if(q_precendence<=stack_precendence){
                    //Pop stack operator and push it to postfix
                    postfix.push_back(op_stack.pop());
                }
                else{
                    //Queue is bigger, put Queue on top of stack operator
                    op_stack.push(q_oper);

                    //Pop front of Infix
                    infix.erase(0);
                }

            }
            break;
        }
        case LPAR:

            //Push Left Parenthesis on top of stack
            op_stack.push(infix[0]);

            //Pop front of Infix
            infix.erase(0);
            break;
        case RPAR:

            //Grab top operator from top of stack
            string stack_oper=op_stack.top();

            //While the top of the stack is not the LPar
            while(grab_type(stack_oper)!=LPAR){
                //Pop the stack and push it to postfix
                postfix.push_back(op_stack.pop());

                //Grab next operator
                stack_oper=op_stack.top();

            }

            //If the stack is not empty, pop the LPar out too
            if(!op_stack.empty()){
                op_stack.pop();
            }

            //Pop front of infix
            infix.erase(0);
            break;
        }

    }//While Loop

    //Pop all remaining operators and push them into postfix
    while(op_stack.empty()!=true){
        postfix.push_back(op_stack.pop());
    }

    return postfix;  //Return postfix



}

int Shunting_yard::grab_type(const string &token){
    /**
       * @brief DEBUG
       * function:grab type
       * purpose:Return an int/enum for token
       * parameters:none
       */

    //If the operators map contains it, return its enum
    if(operators.contains(token)){
        return operators[token];
    }
    //If not an operator, it is a symbol
    else{
        return SYMBOL;
    }
}


void Shunting_yard::set_operators(){
    /**
       * @brief DEBUG
       * function:set operators
       * purpose:Set up operators map
       * parameters:none
       */
    operators["<"]=OP;
    operators["<="]=OP;
    operators[">"]=OP;
    operators[">="]=OP;
    operators["and"]=OP;
    operators["or"]=OP;
    operators["="]=OP;
    operators["("]=LPAR;
    operators[")"]=RPAR;
}

void Shunting_yard::set_precedence(){
    /**
       * @brief DEBUG
       * function:Set Precendence
       * purpose:Set the precendence of each operator
       * in a map
       * parameters:none
       */
    precendence["<"]=4;
    precendence["<="]=4;
    precendence[">"]=4;
    precendence[">="]=4;
    precendence["and"]=3;
    precendence["or"]=2;
    precendence["="]=4;
}
