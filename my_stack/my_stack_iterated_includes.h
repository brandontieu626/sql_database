#ifndef MY_STACK_ITERATED_INCLUDES_H
#define MY_STACK_ITERATED_INCLUDES_H
#include "../../../CS3A/!includes/list_functions_includes/linked_list_functions_includes.h"
template <typename T>
class MyStack
{
public:
    class Iterator{
    public:
        friend class MyStack;
        Iterator (){
            /**
             * @brief DEBUG
             * function: Default CTOR for Iterator
             * purpose: Set _p=NULL
             * parameters: none
             */
            _p=NULL;
        }
        Iterator(node<T>* p){
            /**
             * @brief DEBUG
             * function: CTOR for Iterator
             * purpose: set _p=p
             * parameters: node<T>* p
             */
            _p=p;
        }

        T& operator *(){
            /**
             * @brief DEBUG
             * function:Dereference Operator for Iterator
             * purpose: Return the item in the node
             * Return by reference
             * parameters: none
             */
            return _p->_item;
        }

        T* operator->(){
            /**
             * @brief DEBUG
             * function:Member Access Operator for Iterator
             * purpose: Return address of item in node
             * parameters: none
             */
            return &_p->_item;
        }

        bool is_null(){
            /**
             * @brief DEBUG
             * function Is Null
             * purpose: Return true if _p is at NULL
             * Return false is _p does not
             * parameters: none
             */
            if(_p==NULL){
                return true;
            }
            return false;
        }

        friend bool operator !=(const Iterator& left,const Iterator &right){
            /**
             * @brief DEBUG
             * function:Not Equal to Comparison
             * purpose: compare left iterator!=right iterator
             * Return true if left._p!=right._p
             * Return false if left._p==right._p
             * parameters: const Iterator& left,const Iterator& right
             */
            //Check if addresses are not the same
            if(left._p!=right._p){
                return true;    //Return true
            }
            return false;
        }

        friend bool operator ==(const Iterator& left,
                             const Iterator& right){
            /**
             * @brief DEBUG
             * function: Equality Comparison
             * purpose: Compare left iterator==right iterator
             * Return true if left._p==right._p
             * Return false if left._p!=right._p
             * parameters: const Iterator& left, const Iterator& right
             */
            //check if addresses are the same
            if(left._p==right._p){
                return true;     //return true
            }
            return false;
        }

        Iterator& operator ++(){
            /**
             * @brief DEBUG
             * function: Prefix ++
             * purpose: Move _p->_p->_next
             * Return *this(iterator)
             * (This is changed first then used in other assignments)
             * parameters: none
             */
            assert(_p!=NULL);
            _p=_p->_next; //Move pointer to next
            return *this; //return *this(iterator)
        }

        friend Iterator operator ++(Iterator& it,int unused){
            /**
             * @brief DEBUG
             * function:Postfix ++
             * purpose: Move _p->_p->_next
             * Hold original iterator
             * Move iterator._p->iterator._p->_next
             * Return original iterator
             * parameters: Iterator& it, int unused
             */
            assert(it._p!=NULL);
            Iterator it_hold=it;  //Hold the current iterator
            it._p=it._p->_next;   //Move pointer to next

            return it_hold;       //Return hold iterator
        }

    private:
        node<T>* _p;
    };

    //Default CTOR
    MyStack();

    //Big 3 Functions
    ~MyStack();
    MyStack(const MyStack& rhs);
    MyStack& operator =(const MyStack& rhs);


    void push(T item);  //Push item to front
    T pop();            //Pops Head/Front
    bool empty();       //Is the stack empty?
    T top();            //Returns item at the top

    Iterator Begin();   //Return Iterator(Head)
    Iterator End();     //Return Iterator(End)
    //Ostream operator
    template <typename U>
    friend ostream& operator <<(ostream& outs,const MyStack<U> &Stack1);

private:
    node<T>* head;     //Points to front of stack
};

template <typename T>
MyStack<T>::MyStack(){
    /**
       * @brief DEBUG
       * function: Default CTOR
       * purpose:
       * head=NULL
       * parameters:none
       */
    head=NULL;
}

template <typename T>
MyStack<T>::~MyStack(){
    /**
       * @brief DEBUG
       * function: Destructor for MyStack
       * purpose: Free space from nodes allocated by head
       * parameters: none
       */
    const bool DEBUG=false;
    //Deletes every node
    _ClearList(head);

    if(DEBUG){
        cout<<"~MyStack()"<<endl;
    }

}

template <typename T>
MyStack<T>::MyStack(const MyStack& copythis){
    /**
       * @brief DEBUG
       * function: Copy CTOR for MyStack
       * purpose: Copy copythis into head
       * Calls _CopyList
       * parameters: const MyStack& copythis
       */

    head=NULL;

    //Copy copythis.head into head
    _CopyList(copythis.head,head);
}

template <typename T>
MyStack<T>& MyStack<T>::operator =(const MyStack& rhs){
    /**
       * @brief DEBUG
       * function: Assignment operator for MyStack
       * purpose: Assign RHS MyStack equal to LHS MyStack
       * Clear the nodes at head
       * Copy the nodes from rhs.head to head
       * parameters:const MyStack& rhs
       */
    //Self reference
    if(this==&rhs){
        return *this;
    }

    //Clear all nodes
    _ClearList(head);

    //Copy rhs.head to head
    _CopyList(rhs.head,head);

    //Return itself
    return *this;

}

template <typename T>
void MyStack<T>::push(T item){
    /**
       * @brief DEBUG
       * function: Push back
       * purpose:Push back to front of MyStack
       * Calls _InsertHead
       * parameters:T item
       */
    //Insert head
    _InsertHead(head,item);

}

template <typename T>
T MyStack<T>::pop(){
    /**
       * @brief DEBUG
       * function: Pop
       * purpose:Pop at front/head
       * Return item that was popped
       * Calls _RemoveHead
       * parameters:none
       */
    //Assert if trying to pop an empty stack
    if(empty()){
        assert (1==2);
    }

    //Remove Head
    return _RemoveHead(head);

}

template <typename T>
bool MyStack<T>::empty(){
    /**
       * @brief DEBUG
       * function: Check if MyStack is empty
       * purpose:Return true if MyStack is empty
       * Return false if not empty
       * parameters:none
       */
    //If head is NULL, it is empty
    if(head==NULL){
        return true;
    }

    //Otherwise not empty return false
    return false;

}

template <typename T>
T MyStack<T>::top(){
    /**
       * @brief DEBUG
       * function: Return item at top/front of MyStack
       * purpose: Return item at top/front of MyStack
       * If empty, return NULL
       * parameters:
       */
    if(empty()){
        return NULL;
    }
    return head->_item;
}

template <typename T>
typename MyStack<T>::Iterator MyStack<T>::Begin(){
    /**
       * @brief DEBUG
       * function: Begin
       * purpose: Return Iterator to head
       * parameters:none
       */

    return Iterator(head);
}

template <typename T>
typename MyStack<T>::Iterator MyStack<T>::End(){
    /**
       * @brief DEBUG
       * function: Begin
       * purpose: Return Iterator to NULL
       * parameters: none
       */
    return Iterator(NULL);
}

template <typename U>
ostream& operator <<(ostream& outs, const MyStack<U> &Stack1){
    /**
       * @brief DEBUG
       * function: Ostream operator MyStack
       * purpose:Calls _PrintList
       * parameters:none
       */
    //Print stack
    _PrintList(Stack1.head);

    return outs;
}


#endif // MY_STACK_ITERATED_INCLUDES_H
