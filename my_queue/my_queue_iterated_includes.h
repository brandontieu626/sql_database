#ifndef MY_QUEUE_ITERATED_INCLUDES_H
#define MY_QUEUE_ITERATED_INCLUDES_H
#include "../../../CS3A/!includes/list_functions_includes/linked_list_functions_includes.h"
template <typename T>
class MyQueue
{
public:
    class Iterator{
    public:
        friend class MyQueue;
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
    MyQueue();

    //Big 3 Functions
    ~MyQueue();
    MyQueue(const MyQueue& rhs);
    MyQueue& operator=(const MyQueue& rhs);


    void push(T item);      //Push item to the back
    T pop();                //Pop at front
    bool empty();           //Is the Queue empty?
    T front();              //Return item at front

    Iterator Begin();       //Return Iterator to head
    Iterator End();         //Return Iterator to NULL

    //Ostream operator
    template <typename U>
    friend ostream& operator <<(ostream& outs, const MyQueue<U> &Queue1);

private:
    node<T>* head;   //points to first node
    node<T>* tail;   //points to last node
};


template <typename T>
MyQueue<T>::MyQueue(){
    /**
       * @brief DEBUG
       * function: Default CTOR for Queue
       * purpose: Set Queue to default values
       * head=NULL;
       * parameters: none
       */
    //Head points to NULL
    head=NULL;
    //Tail points to NULL
    tail=NULL;
}

template <typename T>
MyQueue<T>::~MyQueue(){
    /**
       * @brief DEBUG
       * function: Destructor for MyQueue
       * purpose: Free space allocated for myqueue
       * parameters: none
       */
    const bool DEBUG=false;
    //Deletes every node
    _ClearList(head);

    if(DEBUG){
        cout<<"~Destructor MyQueue()"<<endl;
    }
}
template <typename T>
MyQueue<T>::MyQueue(const MyQueue<T> &copythis){
    /**
       * @brief DEBUG
       * function: Copy CTOR for MyQueue
       * purpose:copy copythis.head over into head
       * parameters: const MyQueue<T> &copythis
       */

    //must point to NULL in Copy CTOR
    head=NULL;
    tail=NULL;

    //Copy over the nodes
    _CopyList(copythis.head,head);

    //Point tail to last node
    tail=_LastNode(head);
}

template <typename T>
MyQueue<T>& MyQueue<T>::operator =(const MyQueue& rhs){
    /**
       * @brief DEBUG
       * function: Assignment Operator
       * purpose: Assign lhs MyQueue equal to rhs MyQueue
       * Return itself
       * parameters: const MyQueue& rhs
       */
    //Self Reference
    if(this==&rhs){
        return *this;
    }

    //Delete every node
    _ClearList(head);

    //Copy nodes over
    _CopyList(rhs.head,head);

    //Point tail to last node
    tail=_LastNode(head);

    //Return itself
    return *this;
}

template <typename T>
void MyQueue<T>::push(T item){
    /**
       * @brief DEBUG
       * function: Push back
       * purpose: Push item to back of the nodes
       * Insert after tail to add node to back of the line
       * Tail points to newly inserted node(back of list)
       * parameters: T item
       */
    //Insert After returns the pointer to the newly inserted node
    //Tail points to the most recently added node (Back of line)
    tail=_InsertAfter(head,tail,item);

}

template <typename T>
T MyQueue<T>::pop(){
    /**
       * @brief DEBUG
       * function: Pop
       * purpose: Pops node at the front
       * Returns item that was popped
       * parameters:none
       */

    //If popping an empty queue, assert
    if(empty()){
         assert(1==2);
    }

    T item_removed;

    //Remove head
    //Get item removed
    item_removed=_RemoveHead(head);

    //Make sure when last node is removed, tail points to NULL
    if(empty()){
        tail=NULL;
    }

    //Return item that was removed
    return item_removed;
}

template <typename T>
bool MyQueue<T>::empty(){
    /**
       * @brief DEBUG
       * function: Check if node is empty
       * purpose: Return true if node is empty
       * Return false if not
       * parameters: none
       */

    //If head is null, it is empty
    if(head==NULL){
        return true;
    }

    //If not return false
    return false;

}

template <typename T>
T MyQueue<T>::front(){
    /**
       * @brief DEBUG
       * function: Return item at front
       * purpose: Return item at front of Queue
       * If empty, return NULL
       * parameters: none
       */
    //Check if it is empty
    if(empty()){
        return NULL;
    }

    //Return item at head/front
    return head->_item;
}

template <typename T>
typename MyQueue<T>::Iterator MyQueue<T>::Begin(){
    /**
       * @brief DEBUG
       * function: Begin
       * purpose: Return Iterator to head
       * parameters:none
       */

    return Iterator(head);
}

template <typename T>
typename MyQueue<T>::Iterator MyQueue<T>::End(){
    /**
       * @brief DEBUG
       * function: Begin
       * purpose: Return Iterator to NULL
       * parameters: none
       */

    return Iterator(NULL);
}

template <typename U>
ostream& operator <<(ostream& outs, const MyQueue<U> &Queue1){
    /**
       * @brief DEBUG
       * function: Ostream operator for Queue
       * purpose: Calls _PrintList
       * parameters:none
       */
    //Print queue
    _PrintList(Queue1.head);

    return outs;
}




#endif // MY_QUEUE_ITERATED_INCLUDES_H
