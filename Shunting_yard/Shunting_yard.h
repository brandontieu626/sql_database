#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include "../../../CS3A/!includes/vector_class_includes/vector.h"
#include "../../!includes/bptmap_class/bptmap_class.h"
#include "../../../CS3A/!includes/stack_and_queue_iterated_includes/my_stack_iterated_includes.h"
#include "../../../CS3A/!includes/stack_and_queue_iterated_includes/my_queue_iterated_includes.h"
class Shunting_yard
{
public:
    enum OPERATORS{SYMBOL,OP,LPAR,RPAR};
    Shunting_yard();

    //Infix-> Postfix
    Vector<string> to_postfix(Vector<string> infix);

    //Grabs type
    int grab_type(const string& token);

    //Set up operators map
    void set_operators();

    //Set up precendence map
    void set_precedence();
private:
    Map<string,int> operators;  //Map to tell you which tokens are what
    Map<string,int> precendence;//Map to tell you the precendence of operators
};

#endif // SHUNTING_YARD_H
