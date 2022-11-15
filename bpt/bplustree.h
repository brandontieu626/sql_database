#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "../../!includes/array_utility_functions/array_utility_functions.h"
#include <iostream>
#include <iomanip>
#include <cassert>
using namespace std;

template <class T>
class BPlusTree
{
public:

    class Iterator{
      public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL,int _key_ptr=0):it(_it),key_ptr(_key_ptr){}

        T operator *(){
            /**
               * @brief DEBUG
               * function:Dereference operator
               * purpose:Return data at keyptr
               * parameters:none
               */
            assert(key_ptr<it->data_count);

            return it->data[key_ptr];


        }

        //tested
        Iterator operator ++(int un_used){
            /**
               * @brief DEBUG
               * function:Postfix++
               * purpose:Increment iterator through list
               * When moving to next BTreeNode, key_ptr=0
               * it=it->next
               * parameters:int un_used
               */
            //Increment key ptr
            assert(it!=NULL);
            key_ptr++;

            //If key ptr is >=data count, it is at the next node
            if(key_ptr>=it->data_count){
                key_ptr=0;
                it=it->next;
            }

            return *this;
        }

        //tested
        Iterator operator ++(){
            /**
               * @brief DEBUG
               * function:Prefix ++
               * purpose:Increment iterator through list
               * When moving to next BTreeNode, key_ptr=0
               * parameters:none
               */
            assert(it!=NULL);
            //Increment key ptr
            ++key_ptr;

            //If key ptr is >=data count, it is at the next node
            if(key_ptr>=it->data_count){
                key_ptr=0;
                it=it->next;
            }

            return *this;

        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            /**
               * @brief DEBUG
               * function:== operator
               * purpose:return lhs.it==rhs.it
               * parameters:const Iterator& lhs, const Iterator& rhs
               */
            return ((lhs.it==rhs.it)&&(lhs.key_ptr==rhs.key_ptr));

        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            /**
               * @brief DEBUG
               * function:!=
               * purpose:
               * parameters:const Iterator& lhs, const Iterator& rhs
               */
            //If they aren't pointing to the same node
            //Return true
            if(lhs.it!=rhs.it){
                return true;
            }
            else{
                if(lhs.key_ptr!=rhs.key_ptr){
                    return true;
                }
                else{
                    return false;
                }
            }
        }


        void print_Iterator(){
            /**
               * @brief DEBUG
               * function:Print Iterator
               * purpose:Print key ptr, it data count
               * and where the iterator is in the data
               * parameters:none
               */
            cout<<"Key Ptr: "<<key_ptr<<endl;
            cout<<"Data Count: "<<it->data_count<<endl;
            cout<<it->data[key_ptr];
        }

        bool is_null(){return !it;} //tested


    private:
        BPlusTree<T>* it;  //BPlusTree ptr
        int key_ptr;       //Total keys in tree
    };

    BPlusTree(bool dups = false);

    //Big 3
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& rhs);
    BPlusTree(const BPlusTree<T>& copy_me);

    //Search Functions
    Iterator find(const T& entry);
    bool contains(const T& entry) const;

    //Accessing functions
    T& get(const T& entry);
    const T& get (const T& entry) const;
    T& get_existing(const T& item);
    const T& get_existing(const T& item) const;

    //Insert
    bool insert(const T& entry); //tested

    //Remove
    bool remove(const T& entry);

    //Print Tree
    void print_tree(int level=0,ostream& outs=cout) const;

    //Clear Tree
    void clear_tree();               //tested

    //Copy Tree
    void copy_tree(const BPlusTree<T>& other);  //tested

    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me){
        print_me.print_tree(0,outs);
        return outs;
    }

    int size() const;                    //return total elements in all trees

    bool empty() const;                  //true if no children and data

    Iterator lower_bound(const T& key); //Return >=
    Iterator upper_bound(const T& key); //Return first that goes after

    Iterator begin();  //Return iterator to smallest node
    Iterator end();    //Return iterator to NULL

private:

    //Make these public if you want to use manual tree
    static const int MINIMUM=1;
    static const int MAXIMUM=2 * MINIMUM;

    T data[MAXIMUM+1];                   //holds keys

    BPlusTree<T>* subset[MAXIMUM+2];         //subtrees

    int data_count;                      //num of elements in data
    int child_count;                     //num of children
    //To here

    BPlusTree* next;                     //For leafs to point to next node

    bool dups_ok;                        //Dups ok,override
                                         //Dups not ok, dont do anything
    bool is_leaf() const;

    bool loose_insert(const T& entry);   //inserting allowing above max elements

    bool loose_remove(const T& entry);   //removing allowing below minimum elements

    void fix_shortage(int i);            //fix shortage of elements in child i

    BPlusTree<T>* get_smallest_node();   //Return ptr to left most node

    void get_smallest(T& entry);        //entry := leftmost leaf

    void copy(const BPlusTree& other,BPlusTree*& last_node=NULL);

    T* find_ptr(const T& entry);         //Return ptr to entry

    const T* find_ptr(const T& entry) const; //Const return ptr to entry

    void remove_biggest(T& entry);       //remove biggest child of this tree

    void transfer_right(int i);          //transfer last item from (i-1) to start of i

    void transfer_left(int i);           //transfer first item from i+1 to end of i


    void merge_with_next_subset(int i);

    void fix_excess(int i);



};

template <class T>
BPlusTree<T>::BPlusTree(bool dups):data_count(0),child_count(0),
dups_ok(dups),next(NULL){
    /**
       * @brief DEBUG
       * function:BTree CTOR
       * purpose:Create data of MAX+1
       * Create subset MAX+2
       * Set all subset to NULL
       * parameters:
       */

    for(int i=0;i<MAXIMUM+2;i++){
        subset[i]=NULL;
    }

}

template <class T>
BPlusTree<T>::~BPlusTree(){
    /**
       * @brief DEBUG
       * function:~BTree
       * purpose: Deallocate all nodes
       * Except for original BTree root
       * The allocated memory is there still
       * but DC and CC are 0
       * parameters:none
       */
    const bool DEBUG=false;

    clear_tree();

    if(DEBUG){
        cout<<"~BTree()"<<endl;
    }
}


template <class T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& rhs){
    /**
       * @brief DEBUG
       * function:Assignment Operator
       * purpose: Set LHS = RHS
       * Clear LHS
       * Copy RHS
       * parameters:const BTree<T>& rhs
       */
    //Self Reference
    if(this==&rhs){
        return *this;
    }

    //Clear this tree
    clear_tree();

    //Copy right hand side
    copy_tree(rhs);

    dups_ok=rhs.dups_ok;

    //Return this
    return *this;
}

template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& copy_me):data_count(0),child_count(0),
next(NULL),dups_ok(copy_me.dups_ok){
    /**
       * @brief DEBUG
       * function:Copy CTOR
       * purpose: Copy copy_me into this
       * Data Count(0)
       * Child Count(0)
       * parameters:const BTree<T>& copy_me
       */

    for(int i=0;i<MAXIMUM+2;i++){
        subset[i]=NULL;
    }

    copy_tree(copy_me);
}

template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry){
    /**
       * @brief DEBUG
       * function:find
       * purpose:Recursively search through tree
       * to find entry
       * If it does not exist, return NULL
       * Return pointer to item
       * parameters:
       */
    bool found;

    //In this btree, search for first element >= entry
    int i=first_ge(data,data_count,entry);

    //Check if item found is the entry
    if(i==data_count){
        found=false;
    }
    else{
        if(data[i]==entry){
            found=true;
        }
        else{
            found=false;
        }

    }

    //If found
    if(found){
        //And a leaf, return it
        if(is_leaf()){
            return Iterator(this,i);
        }
        //Not a leaf and found, go search in
        //subset[i+1]
        else{
            return subset[i+1]->find(entry);
        }
    }
    //If not
    else{
        //Check if it is a leaf
        if(is_leaf()){
            return Iterator(NULL);
        }
        //Go search in subset[i]
        else{
            return subset[i]->find(entry);

        }
    }




}

template <typename T>
T& BPlusTree<T>::get(const T& entry){
    /**
       * @brief DEBUG
       * function:get
       * purpose:Return reference to item
       * If BTree does not contain item,insert it
       * Return get_existing
       * parameters:
       */
    //If it does not contain the entry
    if(contains(entry)==false){
        //Insert it
        insert(entry);
    }

    //Return address from get existing
    return get_existing(entry);
}

template <typename T>
const T& BPlusTree<T>::get(const T& entry) const{
    /**
       * @brief DEBUG
       * function:get
       * purpose:Return reference to item
       * If BTree does not contain item,insert it
       * Return get_existing
       * parameters:
       */

    //Return address from get existing
    return get_existing(entry);
}

template <typename T>
T& BPlusTree<T>::get_existing(const T& item){
    /**
       * @brief DEBUG
       * function:get existing
       * purpose:Return reference to item
       * If not found,assert
       * Call find
       * parameters:const T& item
       */
    //Find item
    T* ptr=find_ptr(item);

    //If the pointer is not NULL, return item
    if(ptr!=NULL){
        return *ptr;
    }
    //If it isn't found,assert
    else{
        assert(false);
    }
}

template <typename T>
const T& BPlusTree<T>::get_existing(const T& item) const{
    /**
       * @brief DEBUG
       * function:get existing
       * purpose:Return reference to item
       * If not found,assert
       * Call find
       * parameters:const T& item
       */
    //Find item
    const T* ptr=find_ptr(item);

    //If the pointer is not NULL, return item
    if(ptr!=NULL){
        return *ptr;
    }
    //If it isn't found,assert
    else{
        assert(false);
    }

}

template <class T>
bool BPlusTree<T>::contains(const T& entry) const{
    /**
       * @brief DEBUG
       * function:contains
       * purpose: Return boolean if entry is in BTREE
       * parameters:const T& entry
       */
    const T* ptr=find_ptr(entry);

    //Return true if found
    if(ptr!=NULL){
//        cout<<"Contains: "<<entry<<endl;
        return true;
    }
    //Return false
    else{
//        cout<<"Does not Contain: "<<entry<<endl;
        return false;
    }
}

template <class T>
bool BPlusTree<T>::insert(const T& entry){
    /**
       * @brief DEBUG
       * function:insert
       * purpose:Call loose insert to insert
       * Afterwards check if you are too big,
       * if you are copy all elements, zero out
       * original, and have original point to copy
       * parameters:const T& entry
       */

    const bool DEBUG=false;

    bool success_or_fail;

    //Insert it into tree
    success_or_fail=loose_insert(entry);

    //Check if you are too large
    if(data_count>=MAXIMUM+1){
        //Create new node
        BPlusTree<T>* grow=new BPlusTree<T>(dups_ok);

        //Copy Data[] Elements into new node
        copy_array(grow->data,data,grow->data_count,
                   data_count);

        //Copy Subset[] Pointers into new node
        copy_array(grow->subset,subset,grow->child_count,
                   child_count);

        //Empty out Original Data
        data_count=0;

        //Empty out Original Subset
        for(int i=0;i<child_count;i++){
            subset[i]=NULL;
        }

        //Set child count to 1
        child_count=1;

        //Point original subset 0 to old tree
        subset[0]=grow;

        //Call fix excess on the child 0, which is the copy
        //of original
        fix_excess(0);




    }

    if(DEBUG){
        cout<<"Inserting: "<<entry<<endl;
    }

    return success_or_fail;


}

template <class T>
bool BPlusTree<T>::loose_insert(const T& entry){
    /**
       * @brief DEBUG
       * function:loose insert
       * purpose: Insert into correct position
       * in BTree
       * Insert at Leaf Nodes
       * Recursively call fix excess on your child
       * parameters:const T& entry
       */
    bool found;

    //Find first greater than or equal item to entry
    int i=first_ge(data,data_count,entry);

    //Check if it was found
    if(i==data_count){
        found=false;
    }
    else{
        if(data[i]==entry){
            found=true;
        }
        else{
            found=false;
        }
    }

    //If found
    if(found){
        //And a leaf, check what to do
        if(is_leaf()){
            //Dups ok, update it
            if(dups_ok){
                data[i]+=entry;
            }
            //Do not do anything if not ok
            else{

            }
            return false;
        }
        //Not a leaf
        else{
            //go look in subset[i+1]
            found=subset[i+1]->loose_insert(entry);

            //Fix excess on potential fat child
            fix_excess(i+1);

            return found;
        }
    }
    //Not found
    else{
        //Insert at leaf
        if(is_leaf()){
            //insert in an ordered matter
            ordered_insert(data,data_count,entry);
            return true;

        }
        //Continue to try to find its spot
        else{
            //Go look in subset[i]
            found=subset[i]->loose_insert(entry);
            //Fix excess on potential fat child
            fix_excess(i);

            return found;

        }
    }

}

template <class T>
bool BPlusTree<T>::remove(const T& entry){
    /**
       * @brief DEBUG
       * function:remove
       * purpose:call loose remove
       * If the original root has only 1 child and 0 data
       * Shrink the tree
       * parameters:const T& entry
       */
   const bool DEBUG=false;

   if(DEBUG){
       cout<<"REMOVING: "<<entry<<endl;
   }

   bool success_or_fail;

   //Call loose remove on entry
   success_or_fail=loose_remove(entry);

   //If the data is empty and child count is 1 on original root
   if(data_count==0 && child_count==1){
       //Hold the only child
       BPlusTree<T>* hold_child=subset[0];

       //Copy child data into original data
       copy_array(data,hold_child->data,data_count,hold_child->data_count);

       //Copy child subsets into original subset
       copy_array(subset,hold_child->subset,child_count,
                  hold_child->child_count);

       //Set data and child count to 0 for child
       hold_child->child_count=0;
       hold_child->data_count=0;

       //Delete him
       delete hold_child;

   }

   return success_or_fail;

}

template <class T>
bool BPlusTree<T>::loose_remove(const T& entry){
    /**
       * @brief DEBUG
       * function:loose remove
       * purpose:removing the entry allowing a shortage
       * Leave the fixing up to fix shortage
       * parameters:const T& entry
       */
    bool found;

    int i=first_ge(data,data_count,entry);

    //Check if it was found
    if(i==data_count){
        found=false;
    }
    else{
        if(data[i]==entry){
            found=true;
        }
        else{
            found=false;
        }

    }

    //If found
    if(found){
        //And it is a leaf, delete it
        if(is_leaf()){
            T entry;
            delete_item(data,i,data_count,entry);
            return true;
        }
        //If found and not a leaf, go search in subset[i+1]
        else{
            found=subset[i+1]->loose_remove(entry);

            fix_shortage(i+1);

            return found;
        }
    }
    //If not found
    else{
        //And it is a leaf, not here
        if(is_leaf()){
            return false;
        }
        //And not a leaf, keep searching regularly
        else{
            found=subset[i]->loose_remove(entry);

            fix_shortage(i);

            return found;
        }

    }

}

template <class T>
void BPlusTree<T>::fix_shortage(int i){
    /**
       * @brief DEBUG
       * function:fix shortage
       * purpose:Fix shortage from removal
       * parameters:int i
       */


    //Check if this child has less than min
    if(subset[i]->data_count<MINIMUM){
        //Check to borrow from left/Rotate Right
        //I am not the most left node, and the left node has more than the minimum
        //to give
        if((i-1>=0) && (subset[i-1]->data_count>MINIMUM)){
//            cout<<"ROTATING RIGHT"<<endl;
            transfer_right(i);
        }
        //Check to borrow from right/Rotate Left
        //Check if last child of right and right node has something to give
        else if((i+1<child_count) && (subset[i+1]->data_count>MINIMUM)){
//            cout<<"ROTATING LEFT"<<endl;
            transfer_left(i);

        }
        //Merge
        else{

            //If you are the right most child already
            //Use your left child to merge the right to it
            if(i+1>=child_count){
//                cout<<"MERGE WITH LEFT"<<endl;
                merge_with_next_subset(i-1);
            }
            //If you are not the right most,
            //merge the right of you to yourself
            else{
//                cout<<"MERGE WITH RIGHT"<<endl;
                merge_with_next_subset(i);
            }

        }



    }
}


template <class T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){
    /**
       * @brief DEBUG
       * function:get smallest node
       * purpose:return the smallest node
       * left most node
       * parameters:none
       */

    BPlusTree<T>* ptr=this;
    //Go left while it is not a leaf
    while(ptr->child_count!=0){
        ptr=ptr->subset[0];
    }

    //Return ptr
    return ptr;
}

template <class T>
void BPlusTree<T>::get_smallest(T& entry){
    /**
       * @brief DEBUG
       * function:get smallest node
       * purpose:return the smallest node
       * left most node
       * parameters:none
       */

    BPlusTree<T>* ptr=this;
    //Go left while it is not a leaf
    while(ptr->child_count!=0){
        ptr=ptr->subset[0];
    }

    entry=ptr->data[0];


}

template <class T>
void BPlusTree<T>::remove_biggest(T& entry){
    /**
       * @brief DEBUG
       * function:remove biggest
       * purpose:swap entry with his biggest
       * remove the biggest from where he was
       * parameters:
       */
    //Swap last elements
    if(is_leaf()){

        //Delete item
        delete_item(data,data_count-1,data_count,entry);

    }
    else{
        //Go to most right childs remove biggest
        subset[child_count-1]->remove_biggest(entry);
        //Fix shortage
        fix_shortage(child_count-1);

    }
}


template <class T>
void BPlusTree<T>::fix_excess(int i){
    /**
       * @brief DEBUG
       * function:Fix excess
       * purpose: If child[i] is too large
       * Insert new node to right of large child
       * Split half of data and subset of large child,
       * give it to new node.
       * Take out last element of large child, and pass it to parent.
       * parameters:int i
       */
    T detached;
    //I'm the parent
    //Check size of child
    if((subset[i]->data_count)>=(MAXIMUM+1)){
        //Insert new node to right of fat child
        insert_item(subset,i+1,child_count,new BPlusTree<T>(dups_ok));

        //Split half of data array of fat child
        //Put it into new child
        split(subset[i]->data,subset[i]->data_count,
              subset[i+1]->data,subset[i+1]->data_count);

        //Split subset's subset into new child's subset
        split(subset[i]->subset,subset[i]->child_count,
              subset[i+1]->subset,subset[i+1]->child_count);

        //Take out last element of fat child,
        //Insert it at data[]/parent me
        detach_item(subset[i]->data,subset[i]->data_count,detached);

        //Ordered insert at data[]/parent
        ordered_insert(data,data_count,detached);

        //If the subset that was fat is a leaf,
        if(subset[i]->is_leaf()){
            //Insert detached item that was inserted into the parent
            //Insert it at position 0 of node to the right
            insert_item(subset[i+1]->data,0,subset[i+1]->data_count,
                    detached);

            subset[i+1]->next=subset[i]->next;

            //Point subset[i] to the node right of it
            subset[i]->next=subset[i+1];


        }



    }


}

template <class T>
void BPlusTree<T>::transfer_right(int i){
    /**
       * @brief DEBUG
       * function:transfer right
       * purpose:transfer one element to the right
       * subset[i] is not a leaf, do regular rotate right
       * parameters:int i
       */
    T detached;

    //Detach last item from subset[i-1] data
    detach_item(subset[i-1]->data,subset[i-1]->data_count,
            detached);

    //Insert it at start of subset[i]
    insert_item(subset[i]->data,0,subset[i]->data_count,detached);

    if(subset[i]->is_leaf()){
        //Replace data[i-1] with detached item from subset[i-1]
        data[i-1]=detached;
    }
    else{
        BPlusTree<T>* detached_child;
        int prev_childcount=subset[i-1]->child_count;

        //Swap Data[i-1] and Detached, which was inserted at start of subset[i]
        swap_m(data[i-1],subset[i]->data[0]);

        //Detach last child from left
        detach_item(subset[i-1]->subset,subset[i-1]->child_count,
                    detached_child);

        //Set the pointer to NULL
        subset[i-1]->subset[prev_childcount]=NULL;

        //Insert detached last child from left to start
        //of skinny node(node to the right)
        insert_item(subset[i]->subset,0,subset[i]->child_count,
                    detached_child);

    }

}

template <class T>
void BPlusTree<T>::transfer_left(int i){
    /**
       * @brief DEBUG
       * function:transfer right
       * purpose:transfer one element left
       * If subset[i] is a leaf, rotate left
       * parameters:int i
       */
    T deleted;

    //Delete data[0] from subset[i+1]
    delete_item(subset[i+1]->data,0,subset[i+1]->data_count,deleted);

    //Attach it to subset[i] data
    attach_item(subset[i]->data,subset[i]->data_count,deleted);

    if(subset[i]->is_leaf()){
        //Set data[i]=subset[i+1]->data[0]
        data[i]=subset[i+1]->data[0];
    }
    else{
        BPlusTree<T>* detached_child;

        //Swap subset[i]->data[last element] with data[i]
        swap_m(subset[i]->data[(subset[i]->data_count)-1],data[i]);

        //Delete child 0 from subset[right]
        delete_item(subset[i+1]->subset,0,subset[i+1]->child_count,
                    detached_child);

        //Attach detached child to subset[i]->subset
        attach_item(subset[i]->subset,subset[i]->child_count,detached_child);

    }

}

template <typename T>
void BPlusTree<T>::merge_with_next_subset(int i){
    /**
       * @brief DEBUG
       * function:merge with next subset
       * purpose:merge subset i with next subset
       * Only attach detached item from top node when subset[i] is not a leaf
       * parameters:int i
       */
    T deleted;

    //Delete data[i]
    delete_item(data,i,data_count,deleted);

    //Append data[i] to child[i] data
    if(!subset[i]->is_leaf()){
        attach_item(subset[i]->data,subset[i]->data_count,deleted);
    }

    //Merge right subset data to subset[i] data
    merge(subset[i]->data,subset[i]->data_count,subset[i+1]->data,
            subset[i+1]->data_count);


    //Merge right subset children to subset[i] subset
    merge(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,
            subset[i+1]->child_count);

    //Ptr for node that was removed
    BPlusTree<T>* remove_child;

    //Delete the subset[i+1] from subset
    delete_item(subset,i+1,child_count,remove_child);

    //Delete it from memory now
    delete remove_child;

    //Set subset[i] next to NULL
    subset[i]->next=NULL;


}

template <class T>
void BPlusTree<T>::print_tree(int level,ostream& outs) const{
    /**
       * @brief DEBUG
       * function:print tree
       * purpose:Print all nodes sideways on appropriate levels
       * parameters:int level,ostream& outs
       */
    //If it is not a leaf, print last subtree
    if(!is_leaf()){
        subset[child_count-1]->print_tree(level+1,outs);
    }

    //In loop, print this BTrees data[i] and subset[i]
    //The base case will be when the data count is 0, and it goes back up
    for(int i=data_count-1;i>=0;i--){
        outs<<setw(4*level)<<""<<"["<<data[i]<<"]"<<endl;
        if(subset[i]!=NULL){
            subset[i]->print_tree(level+1,outs);
        }
    }


}

template <class T>
void BPlusTree<T>::clear_tree(){
    /**
       * @brief DEBUG
       * function:Clear Tree
       * purpose: Delete all nodes except original BTree
       * parameters:none
       */
    //Set data count to 0
    data_count=0;

    //For loop to delete all children
    //Base case is when child count is 0
    for(int i=0;i<child_count;i++){

        subset[i]->clear_tree();

        //Delete the node
        delete subset[i];

        //Set ptr to NULL
        subset[i]=NULL;
    }

    //Set child count to 0 deleting all subsets
    child_count=0;


}

template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){
    /**
       * @brief DEBUG
       * function:copy tree
       * purpose:Create a ptr to pass to private copy
       * parameters:const BPlusTree<T>& other
       */
    BPlusTree<T>* last_node=NULL;

    copy(other,last_node);


}


template <class T>
void BPlusTree<T>::copy(const BPlusTree& other,BPlusTree*& last_node){
    /**
       * @brief DEBUG
       * function:Copy
       * purpose: Copy other BTree into this BTree
       * Connect the linked list at leaf level
       * Pass a ptr to connect the list
       * parameters:const BTree<T>& other
       */
    //Copy array
    copy_array(data,other.data,data_count,
               other.data_count);

    //For loop that recursively copies
    for(int i=0;i<other.child_count;i++){
        //Create a new node at child position
        subset[i]=new BPlusTree<T>(dups_ok);
        //Call copy tree on child giving it the other BTree's child
        subset[i]->copy(*other.subset[i],last_node);
    }

    //Set your count= to other's count
    child_count=other.child_count;

    //If I am a leaf
    if(is_leaf()){
        //If i am the first leaf
        if(last_node==NULL){
            //Point last node to me
            last_node=this;
        }
        else{
            //If i am not the last node,
            //connect the last node's next to me
            last_node->next=this;
            //Make the next last node me
            last_node=this;
        }

    }

}

template <class T>
T* BPlusTree<T>::find_ptr(const T& entry){
    /**
       * @brief DEBUG
       * function:find ptr
       * purpose:Recursively search through tree
       * to find entry
       * If it does not exist, return NULL
       * Return pointer to item
       * parameters:
       */
    bool found;

    //In this btree, search for first element >= entry
    int i=first_ge(data,data_count,entry);

    //Check if item found is the entry
    if(i==data_count){
        found=false;
    }
    else{
        if(data[i]==entry){
            found=true;
        }
        else{
            found=false;
        }

    }

    //If found
    if(found){
        //And a leaf, return it
        if(is_leaf()){
            return &data[i];
        }
        //Not a leaf and found, go search in
        //subset[i+1]
        else{
//            cout<<"NL&F:Go search in: "<<endl<<*subset[i+1]<<endl;
            return subset[i+1]->find_ptr(entry);
        }
    }
    //If not
    else{
        //Check if it is a leaf
        if(is_leaf()){
            return NULL;
        }
        //Go search in subset[i]
        else{
//            cout<<"Go search in: "<<endl<<*subset[i]<<endl;
            return subset[i]->find_ptr(entry);

        }
    }
}

template <class T>
const T* BPlusTree<T>::find_ptr(const T& entry) const{
    /**
       * @brief DEBUG
       * function:find ptr const
       * purpose:Recursively search through tree
       * to find entry
       * If it does not exist, return NULL
       * Return pointer to item
       * parameters:
       */
    bool found;

    //In this btree, search for first element >= entry
    int i=first_ge(data,data_count,entry);

    //Check if item found is the entry
    if(i==data_count){
        found=false;
    }
    else{
        if(data[i]==entry){
            found=true;
        }
        else{
            found=false;
        }

    }

    //If found
    if(found){
        //And a leaf, return it
        if(is_leaf()){
            return &data[i];
        }
        //Not a leaf and found, go search in
        //subset[i+1]
        else{
//            cout<<"NL&F:Go search in: "<<endl<<*subset[i+1]<<endl;
            return subset[i+1]->find_ptr(entry);
        }
    }
    //If not
    else{
        //Check if it is a leaf
        if(is_leaf()){
            return NULL;
        }
        //Go search in subset[i]
        else{
//            cout<<"Go search in: "<<endl<<*subset[i]<<endl;
            return subset[i]->find_ptr(entry);

        }
    }
}

template <class T>
bool BPlusTree<T>::is_leaf() const{
    /**
       * @brief DEBUG
       * function:is leaf
       * purpose: If child count=0,return true
       * if not return false
       * parameters:none
       */
    if(child_count==0){
        return true;
    }

    return false;
}

template <class T>
int BPlusTree<T>::size() const{
    /**
       * @brief DEBUG
       * function:size
       * purpose:return size of every element in tree
       * parameters:none
       */

    //If it is a leaf return your data count
    if(is_leaf()){
        return data_count;
    }
    else{
        int count=data_count;

        //Count each subsets size and return it
        for(int i=0;i<child_count;i++){
            count+=subset[i]->size();
        }

        return count;
    }


}

template <class T>
bool BPlusTree<T>::empty() const{
    /**
       * @brief DEBUG
       * function:empty
       * purpose:return true if tree is empty
       * return false otherwise
       * parameters:
       */
    if(child_count==0&&data_count==0){
        return true;
    }
    else{
        return false;
    }
}

template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key){
    /**
       * @brief DEBUG
       * function:Lower Bound
       * purpose:Return Iterator to Key if found
       * If not found, return the key > than it
       * parameters:const T& key
       */

    //Search for the key
    Iterator search=find(key);

    //If it was found, return the iterator
    if(search!=end()){
        return search;
    }
    //If not, return the item greater than it
    else{
        return upper_bound(key);
    }

}

template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key){
    /**
       * @brief DEBUG
       * function:Upper Bound
       * purpose:Return Iterator to item greater than key
       * If item is greater than greatest item, returns Null Iterator
       * parameters:const T& key
       */

    bool found;

    //In this btree, search for first element >= entry
    int i=first_ge(data,data_count,key);


    //Check if item found is the entry
    if(i==data_count){
        found=false;
    }
    else{
        if(data[i]==key){
            found=true;
        }
        else{
            found=false;
        }

    }

    //If found
    if(found){
        //If it is found, return the Iterator to item after it
        if(is_leaf()){
            Iterator upper(this,i);

            upper++;

            return upper;

        }
        //Not a leaf and found, go search in
        //subset[i+1]
        else{
            return subset[i+1]->upper_bound(key);
        }
    }
    //If not
    else{
        //If not found and at a leaf
        if(is_leaf()){
            //If it is the same as the data count
            if(i==data_count){
                //Check if the iterator before it is equal to NULL
                Iterator current(this,i-1);
                current++;
                //If it is not NULL, return this Iterator
                if(current!=end()){
                    return current;
                }
                //Return NULL Iterator
                else{
                    return Iterator(NULL);
                }
            }
            //If it is not the data count, just return this Iterator
            else{
                return Iterator(this,i);
            }

        }
        //Go search in subset[i]
        else{
            return subset[i]->upper_bound(key);

        }
    }


}

template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    /**
       * @brief DEBUG
       * function:Begin
       * purpose:Return iterator of smallest node
       * parameters:none
       */

    return Iterator(get_smallest_node());
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    /**
       * @brief DEBUG
       * function:End
       * purpose:Return iterator of NULL/End of tree
       * parameters:none
       */
    return Iterator(NULL);
}

#endif // BPLUSTREE_H
