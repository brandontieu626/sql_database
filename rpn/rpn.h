#ifndef RPN_H
#define RPN_H
#include "../../../CS3A/!includes/stack_and_queue_iterated_includes/my_stack_iterated_includes.h "
#include "../../!includes/vector_heap_sort/heap_sort.h"
#include "../../!includes/bptmmap_class/bptmmap_class.h"
#include "../../!includes/bptmap_class/bptmap_class.h"
class RPN
{
public:
    enum TYPES{SYMBOL,OPERATOR,LOGOPERATOR};
    RPN(Vector<string> postfix);

    //Evaluate function
    Vector<long> evaluate(Map<string,int> &positions,Vector<MMap<string,long>> &indices);

    //Evaluate <,>,<=,>=,=
    Vector<long> evaluate_op(string field,string op,string value,
                             Map<string,int> &positions,Vector<MMap<string,long>> &indices);

    //Evaluate and/or
    Vector<long> evaluate_op(string op,Vector<long>& first,Vector<long>& second);

    //Returns type
    int grab_type(const string& token);

    //Sets up map
    void set_up_types();
private:
    Vector<string> _postfix;      //Hold postfix string
    Map<string,int> _types;       //Map for types of symbols/operators in postfix

};

#endif // RPN_H
