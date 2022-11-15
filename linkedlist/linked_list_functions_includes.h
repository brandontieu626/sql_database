#ifndef LINKED_LIST_FUNCTIONS_INCLUDES_H
#define LINKED_LIST_FUNCTIONS_INCLUDES_H
#include "node_includes.h"
#include <iostream>
#include <assert.h>
using namespace std;

//Function Prototypes/Declaration
template <typename T>
void _PrintList(node<T>* head);

template <typename T>
node<T>* _InsertHead(node <T>* &head,T item);

template <typename T>
node<T>* _SearchList(node<T>* head,T key);

template <typename T>
node<T>* _InsertAfter(node<T>* &head, node<T>* after_this,
                     T insert_this);

template <typename T>
node<T>* _InsertBefore(node<T>* &head, node<T>* before_this,
                     T insert_this);
template <typename T>
T _RemoveHead(node<T>* &head);

template <typename T>
node<T>* _PreviousNode(node<T>* head, node<T>* before_this);

template <typename T>
T _DeleteNode(node<T>* &head,node<T>* deleteThis);

template <typename T>
node<T>* _CopyList(node<T>* head,node<T>* &copy);

template <typename T>
void _ClearList(node<T>* &head);

template <typename T>
T& _At(node<T>* head,int pos);

template <typename T>
node<T>* _LastNode(node<T>* head);


//Sorted Functions
template <typename T>
node<T>* InsertSorted(node<T>* &head,T item, bool ascending);

template <typename T>
node<T>* InsertSorted_and_add(node<T>* &head,T item,bool ascending=true);

template <typename T>
node<T>* WhereThisGoes(node<T>* head,T item, bool ascending=true);







//Function Definitions
template <typename T>
void _PrintList(node<T>* head){
    /**
     * @brief DEBUG
     * function: Print list of nodes
     * purpose: Go through each node and print it
     * stopping at null
     * parameters: node<T>* head ptr
     */
    //While head does not point to null,
    //print each node
    while(head!=NULL){
        cout<<"["<<*head<<"]->";
        head=head->_next;
    }

    //Print ||| to indicate NULL
    if(head==NULL){
        cout<<"|||";
    }


}

template <typename T>
node<T>* _InsertHead(node <T>* &head,T item){
    /**
     * @brief DEBUG
     * function: Insert Head
     * purpose:Insert new node as head
     * Create new node and insert it as head
     * Head points to new node
     * Return new node
     * parameters: node<T>* &headptr, T item
     */
    //Create new node that is being inserted
    node<T>* insert_ptr=new node<T>(item);

    //Point new items next to head
    insert_ptr->_next=head;
    //Point head to new node
    head=insert_ptr;

    //Return new node
    return insert_ptr;

}

template <typename T>
node<T>* _SearchList(node<T>* head,T key){
    /**
     * @brief DEBUG
     * function: Search List
     * purpose: Search list of nodes for node containing key
     * Return pointer to node containing key if found
     * Return NULL if not found
     * If head is NULL, return NULL
     * parameters: node<T>* head,T key
     */
    while(head!=NULL){
        if(head->_item==key){
            return head;
        }

        head=head->_next;
    }

    return NULL;
}

template <typename T>
node<T>* _InsertAfter(node<T>* &head, node<T>* after_this,
                     T insert_this){
    /**
     * @brief DEBUG
     * function:Insert Node After pointer position
     * purpose: Insert Node after after_this position
     * Return pointer to inserted item
     * If Head Or After This is NULL, Insert Head
     * parameters:node<T>* headptr, node<T>* after this node, T item
     */
    //Node walker for head
    node<T>* node_walker=head;
    //Create new node to insert
    node<T>* inserted_item_ptr;

    //If head is NULL, nothing to insert after, just insert head
    if(head==NULL){
        //Insert head
        inserted_item_ptr=_InsertHead(head,insert_this);
        //Return pointer to inserted item
        return inserted_item_ptr;
    }
    else{
         inserted_item_ptr=new node<T>(insert_this);

    }


    //Move walker to where you want to insert after
    while(node_walker!=after_this&&node_walker!=NULL){
        node_walker=node_walker->_next;
    }

    //Point inserted item to next node in list
    inserted_item_ptr->_next=node_walker->_next;
    //Point node behind it to inserted item
    node_walker->_next=inserted_item_ptr;

    //Return inserted item
    return inserted_item_ptr;

}

template <typename T>
node<T>* _InsertBefore(node<T>* &head, node<T>* before_this,
                      T insert_this){
    /**
     * @brief DEBUG
     * function: Insert node Before pointer position
     * purpose:Insert node before before_this position
     * calls _PreviousNode
     * Return pointer to inserted item
     * If before this is head, InsertHead
     * If head is NULL, InsertHead
     * parameters: node<T>* headptr, node<T>* before this node, T item
     */
    //Pointer to inserted item
    node<T>* inserted_item_ptr;
    //Pointer to previous node before before_this
    node<T>* previous_before_this;

    //Search for the node before before_this
    previous_before_this=_PreviousNode(head,before_this);

    //the previous node is the head, just inserthead
    if(before_this==head){
        //Insert Head
        inserted_item_ptr=_InsertHead(head,insert_this);
        //return pointer to inserted item
        return inserted_item_ptr;
    }

    //Insert after node before before_this node
    inserted_item_ptr=_InsertAfter(head,previous_before_this,insert_this);

    //Return ptr to inserted item
    return inserted_item_ptr;


}

template <typename T>
T _RemoveHead(node<T>* &head){
    /**
     * @brief DEBUG
     * function: Remove node at head position
     * purpose: Remove node at head position
     * Return item that was removed
     * If head ==NULL, return NULL
     * parameters: node<T>* headptr
     */


    if(head!=NULL){
        //temporary to point at item being removed
        node<T>* temp=head;


        //Item being removed
        T item;


        //Head points to next node
        head=head->_next;


        //Save item from node being removed
        item=temp->_item;

        //Delete node
        delete temp;


        //Return item
        return item;


    }

    return T();
}


template <typename T>
node<T>* _PreviousNode(node<T>* head, node<T>* before_this){
    /**
     * @brief DEBUG
     * function:Find previous node
     * purpose: Find node previous to node before_this
     * return pointer to node previous to node before_this
     * If before this is head, return NULL
     * If head is null, return NULL
     * parameters: node<T>* head_ptr, node<T>* before this
     */

    //If before this is head or if head is NULL, return NULL
    //Nothing behind head
    if(before_this==head||head==NULL){
        return NULL;
    }

    //Search through head to find previous node
    while(head!=NULL){
        //If the next node is before this,
        //you have found the node before before this
        if(head->_next==before_this){
            return head;
        }
        //move head through list
        head=head->_next;
    }


    //If they are giving you a node not even in the list, assert
    assert(1==2);

}

template <typename T>
T _DeleteNode(node<T>* &head,node<T>* deleteThis){
    /**
     * @brief DEBUG
     * function: Delete node at deletethis position
     * purpose:Delete node at delete this position
     * Return item that was deleted
     * If PreviousNode Returns Null, RemoveHead because it was the head
     * parameters:node<T>* &head, node<T>* deleteThis
     */

    //Walker for node
    node<T>* node_walker;
    //Item that is deleted
    T deleted_item;

    //Search for node before node being deleted
    node_walker=_PreviousNode(head,deleteThis);

    //If it was NULL, it was the head, so just remove head
    if(node_walker==NULL){
        //Remove head
        deleted_item=_RemoveHead(head);
        //Return item being deleted
        return deleted_item;
    }

    //Remove head
    deleted_item=_RemoveHead(deleteThis);

    //Must connect the previous node to new next node
    node_walker->_next=deleteThis;

    //return item being deleted
    return deleted_item;


}

template <typename T>
node<T>* _CopyList(node<T>* head,node<T>* &copy){
    /**
     * @brief DEBUG
     * function:Copy List of Nodes
     * purpose: Create a copy of nodes
     * Return pointer to last node of copy
     * If head is NULL, return NULL
     * Copy will point to the head of the list of nodes,
     * through InsertAfter
     * parameters: node<T>* headptr, node<T>* &copy
     */

    //If head is NULL, no copy to be made
    if(head==NULL){
        return NULL;
    }

    //Insert after will call inserthead since copy will be empty
    //Copy first element
    node<T>* dest_walker=_InsertAfter(copy,copy,head->_item);

    //move head to next node, have already copied first element
    head=head->_next;
    //Loop through all nodes of head
    while(head!=NULL){
        //Insert after returns a pointer to newly inserted item
        //Loop will insert after each newly inserted item
        //Inserts each node in head
        dest_walker=_InsertAfter(dest_walker,dest_walker,head->_item);
        //Move head to next
        head=head->_next;

    }

    //Return pointer to last node
    return dest_walker;


}

template <typename T>
void _ClearList(node<T>* &head){
    /**
     * @brief DEBUG
     * function: Clear entire list of nodes
     * purpose: Clear entire list of nodes
     * Remove Head while Head!=NULL
     * parameters:node<T>* &headptr
     */
    //Loop through each node in head
    while(head!=NULL){
        //Remove each node
        _RemoveHead(head);

    }

//    cout<<"Head: "<<head<<endl;

}

template <typename T>
T& _At(node<T>* head,int pos){
    /**
     * @brief DEBUG
     * function: Return item at pos
     * purpose:Return item at position
     * parameters: node<T>* head, int pos
     */

    //Increment head to find position
    for(int i=0;i<pos;i++){
        head=head->_next;
    }

    //Return item at position
    return head->_item;
}

template <typename T>
node<T>* _LastNode(node<T>* head){
    /**
     * @brief DEBUG
     * function:Return pointer to last node
     * purpose: Return pointer to last node
     * parameters: node<T>* headptr
     */
    //If head is null, empty so return NULL
    if(head==NULL){
        return NULL;
    }

    //Loop through each element until the nodes next is NULL,
    //meaning that it is last node
    while(head->_next!=NULL){
        //move head through list
        head=head->_next;
    }

    //Return head
    return head;
}






//Sorted Functions
template <typename T>
node<T>* InsertSorted(node<T>* &head,T item, bool ascending){
    /**
     * @brief DEBUG
     * function: InsertSorted
     * purpose: Insert item at correct position for sorted
     * Calls _WhereThisGoes to find where it belongs
     * Inserts item into list, allows duplicates
     * Returns ptr to inserted node
     * parameters: node<T>* &headptr, T item, bool ascending
     */
   //Insert After this pointer
   node<T>* insert_after_this;
   //Pointer to newly inserted item
   node<T>* inserted_item_ptr;

   //Find where item goes in list
   insert_after_this=WhereThisGoes(head,item,ascending);

   //Insert where it belongs
   inserted_item_ptr=_InsertAfter(head,insert_after_this,item);

   //Return ptr to inserted node
   return inserted_item_ptr;
}

template <typename T>
node<T>* InsertSorted_and_add(node<T>* &head,T item,bool ascending){
    /**
     * @brief DEBUG
     * function: InsertSorted and Add
     * purpose: Insert item at correct position for sorted
     * If a duplicate is found,add to the duplicate
     * Returns pointer to newly inserted node
     * parameters: node<T>* &headptr, T item, bool ascending
     */
    //Insert After this pointer
    node<T>* insert_after_this;
    //Pointer to newly inserted item
    node<T>* inserted_item_ptr;

    //Find where item goes in list
    insert_after_this=WhereThisGoes(head,item,ascending);

    //If the insert after this is NULL, it means head was NULL
    //call InsertAfter which will call InsertHead since head is NULL
    if(insert_after_this==NULL){
        inserted_item_ptr=_InsertAfter(head,insert_after_this,item);
        return inserted_item_ptr;
    }

    //If the insert after this item==item,
    if(insert_after_this->_item==item){
        insert_after_this->_item+=item;   //Add items together
        return insert_after_this;         //Return ptr to newly inserted item
    }
    //If they are not the same, insert into the list
    else{
        inserted_item_ptr=_InsertAfter(head,insert_after_this,item); //Insert into list
        return inserted_item_ptr;                                    //Return ptr to newly inserted item
    }



}

template <typename T>
node<T>* WhereThisGoes(node<T>* head,T item, bool ascending){
    /**
     * @brief DEBUG
     * function: WhereThisGoes
     * purpose: Return pointer to where an item belongs in a sorted list
     * Pointer points to before where they belong so you can insert after
     * Returns the follower
     * If Head is NULL, Return NULL
     * parameters: node<T>* headptr, T item, bool ascending
     */
    //Follower starts at NULL
    node<T>* follower=NULL;
    //Leader starts at NULL
    node<T>* leader=head;

    //If the head is NULL,return NULL
    if(head==NULL){
        return NULL;
    }


    //If ascending is true
    if(ascending==true){
        //Continue until the leader goes off into NULL,
        //Last spot can still be viable for where the item can go
        while(leader!=NULL){
            //If item >= than the leader's item, move them along
            if(item>leader->_item){
                follower=leader;
                leader=leader->_next;
            }
            else if(item==leader->_item){
                follower=leader;
                leader=leader->_next;
            }
            //If item<leader's item,
            //found position and return follower
            else{
                return follower;
            }
        }
        //In case the item is the largest and never goes through the else
        return follower;

    }
    //If descending
    else{
        //Continue until leader goes off into NULL
        //Last spot can still be viable for where item can go
        while(leader!=NULL){
            //If item <= than the leader's item move them along
            if(item<leader->_item){
                follower=leader;
                leader=leader->_next;
            }
            else if(item==leader->_item){
                follower=leader;
                leader=leader->_next;
            }
            //If item>leader's item,
            //you have found the position and return follower
            else{
                return follower;
            }
        }
        //In case item is the smallest and never goes through else
        return follower;
    }
}


#endif // LINKED_LIST_FUNCTIONS_INCLUDES_H
