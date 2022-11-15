#include "rpn.h"

RPN::RPN(Vector<string> postfix)
{
    /**
       * @brief DEBUG
       * function:RPN Default CTOR
       * purpose:Copies postfix vectorstr
       * Set up map of types
       * parameters:Vector<string> postfix
       */

    //Copy Postfix
    _postfix=postfix;

    //Set up Map of <string,int>
    set_up_types();

}

Vector<long> RPN::evaluate(Map<string,int> &positions,Vector<MMap<string,long>> &indices){
    /**
       * @brief DEBUG
       * function:evaluate
       * purpose:Evaluate the entire postfix vector
       * Uses evaluate op to evaluate the operators
       * Returns a vector<long> of record numbers
       * parameters:Map<string,int> &positions,Vector<MMap<string,long>> &indices
       */

    //Vector for result
    Vector<long> result;

    //Copy vector
    Vector<string> postfix=_postfix;

    //Stack for strings
    MyStack<string> str_stack;

    //Stack for record numbers
    MyStack<Vector<long>> rpn_stack;

    //Loop until the postfix is empty
    while(!postfix.empty()){
        //See what type the front of the postfix is
        switch(grab_type(postfix[0])){
        case SYMBOL:
            //Push symbols onto the string stack
            str_stack.push(postfix[0]);
            break;
        case OPERATOR:
        {
            //Pop twice and get the operator
            string oper=postfix[0];
            string first_sym=str_stack.pop();
            string second_sym=str_stack.pop();


            //Evaluate op
            result=evaluate_op(second_sym,oper,first_sym,positions,indices);

            //Add resulting vector to stack of record numbers
            rpn_stack.push(result);
            break;
        }
        case LOGOPERATOR:
        {
            //Pop the vector stack twice
            Vector<long> first=rpn_stack.pop();
            Vector<long> second=rpn_stack.pop();

            //Evaluate the operator and pass Logical Operator
            result=evaluate_op(postfix[0],first,second);

            //Push resulting vector into vector stack
            rpn_stack.push(result);
            break;
        }
        }

        //clear result after each run through
        result=Vector<long>();


        //Remove/Pop front
        postfix.erase(0);

    }//While Loop



    //Pop resulting vector in stack
    return rpn_stack.pop();
}

Vector<long> RPN::evaluate_op(string field,string op,string value,
                         Map<string,int> &positions,Vector<MMap<string,long>> &indices){
    /**
       * @brief DEBUG
       * function:evaluate op
       * purpose:Return vector of record numbers given operator,field, and value
       * Requires usage of the positions and indices
       * parameters:string field,string op,string value,
          Map<string,int> &positions,Vector<MMap<string,long>> &indices
       */

    //All records that contain the conditions
    Vector<long> allrecords;

    //If the map contains the field, process it
    if(positions.contains(field)){

        //Find where in indices it is
        int where=positions[field];

        //Equal Sign, Access Indices with Where and Value
        if(op=="="){
            allrecords=indices[where][value];
        }
        else if(op==">"){
            //Find the upper bound
            MMap<string,long>::Iterator upper=indices[where].upper_bound(value);

            //Get the end of the multimap
            MMap<string,long>::Iterator _end=indices[where].end();


            //Loop from the upper bound until the end
            while(upper!=_end){
                //Keep adding records to all records
                allrecords=allrecords+(*upper).value_list;
                upper++;

            }
        }
        else if(op=="<"){
            //Find Lower bound
            MMap<string,long>::Iterator lower=indices[where].lower_bound(value);

            //Find Start of Multimap
            MMap<string,long>::Iterator _begin=indices[where].begin();


            //Loop from the start to the lower bound
            while(_begin!=lower){
                //Keep adding records to keep all records
                allrecords=allrecords+(*_begin).value_list;
                _begin++;
            }
        }
        else if(op=="<="){
            //Find Lower Bound
            MMap<string,long>::Iterator lower=indices[where].lower_bound(value);

            //Find the start
            MMap<string,long>::Iterator _begin=indices[where].begin();

            //Loop from start to Lower bound
            while(_begin!=lower){
                //Add records to keep all records
                allrecords=allrecords+(*_begin).value_list;
                _begin++;
            }

            //If the multimap contains the value, then the lower bound
            //missed the = value
            if(indices[where].contains(value)){
                //If it is not at the end, add the = element to all records
                if(lower!=indices[where].end()){
                    allrecords=allrecords+(*_begin).value_list;

                }
            }

        }
        else if(op==">="){
            //Get Lower Bound
            MMap<string,long>::Iterator lower=indices[where].lower_bound(value);

            //Get the end for multimap
            MMap<string,long>::Iterator _end=indices[where].end();

            //Loop from lower bound to end of multimap
            while(lower!=_end){
                //Keep adding on to keep track of all elements
                allrecords=allrecords+(*lower).value_list;
                lower++;

            }

        }

    }

    //Return allrecords with conditions
    return allrecords;
}

Vector<long> RPN::evaluate_op(string op,Vector<long>& first,Vector<long>& second){
    /**
       * @brief DEBUG
       * function:evaluate op
       * purpose:evaluate a logical operator
       * return the union or intersection of the 2 vectors
       * Both need to be sorted for my algorithms to work
       * parameters:string op,Vector<long>& first,Vector<long>& second
       */
    //Sort both vectors first
    sort(first);
    sort(second);

    //And is an intersection
    if(op=="and"){
        return intersection_m(first,second);
    }
    //Or is a union
    else{     
        return union_m(first,second);
    }
}

int RPN::grab_type(const string &token){
    /**
       * @brief DEBUG
       * function:grab type
       * purpose:Return token's enumeration in the map
       * parameters:const string &token
       */
    //Check if the types map contains it
    if(_types.contains(token)){
        //Return its type
        return _types[token];
    }
    //If not,return a symbol
    else{
        return SYMBOL;
    }
}

void RPN::set_up_types(){
    /**
       * @brief DEBUG
       * function:set up types
       * purpose:Set up Map<string,int> with enumerations
       * parameters:none
       */
    _types[">"]=OPERATOR;
    _types["<"]=OPERATOR;
    _types[">="]=OPERATOR;
    _types["<="]=OPERATOR;
    _types["="]=OPERATOR;
    _types["and"]=LOGOPERATOR;
    _types["or"]=LOGOPERATOR;
}




