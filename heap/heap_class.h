#ifndef HEAP_CLASS_H
#define HEAP_CLASS_H
#include "../../../CS3A/!includes/vector_class_includes/vector.h"
#include <iomanip>
template <typename T>
class Heap{
public:
    Heap();
    Heap (T arr[],unsigned int size);
    Heap(unsigned int capacity);      //Set capacity
    void insert(const T& insert_me);  //Insert into heap
    T Pop();                          //Pop
    void clear();
    int size() const;
    int capacity() const;
    bool is_empty() const;
    //Print heap
    friend ostream& operator <<(ostream& outs, const Heap<T>& print_me){
        print_me.print_tree(0,0,outs);

        return outs;
    }
private:
    unsigned int how_many;  //Size
    Vector<T> _tree;        //Vector for tree

    //Print vector as an array
    void print_tree(ostream& outs=cout) const;
    //Print vector as tree
    void print_tree(unsigned int root=0, int level=0,ostream& outs=cout) const;

    //Return true if a leaf, false if not a leaf
    bool is_leaf(int i) const;

    unsigned int parent_index(unsigned int i) const;
    unsigned int left_child_index(unsigned int i) const;
    unsigned int right_child_index(unsigned int i) const;
    unsigned int big_child_index(unsigned int i) const;
    void swap_with_parent(unsigned int i);
};


template <typename T>
Heap<T>::Heap():how_many(0){
    /**
       * @brief DEBUG
       * function: Default CTOR Heap
       * purpose: set how_many=0
       * parameters: none
       */
    //Empty
}

template <typename T>
Heap<T>::Heap(T arr[],unsigned int size):_tree(size){

    for(int i=0;i<size;i++){
        insert(arr[i]);
    }



}

template <typename T>
Heap<T>::Heap(unsigned int capacity):how_many(0),_tree(capacity){
    /**
       * @brief DEBUG
       * function: CTOR Heap
       * purpose: set _tree capacity given capacity
       * parameters: unsigned int capacity
       */
    //Empty
}

template <typename T>
void Heap<T>::insert(const T& insert_me){
    /**
       * @brief DEBUG
       * function:insert
       * purpose: insert insert_me into tree
       * parameters: const T& insert_me
       */

    //Insert to tree
    _tree+=insert_me;

    //Size ++
    how_many=_tree.size();

    //Newly inserted item is at size-1
    unsigned int inserted=how_many-1;
    //Get newly inserted items parent index
    unsigned int parent=parent_index(inserted);

    //Do swapping
    //Swap with parent while the inserted item is greater than parent
    while(_tree[parent]<_tree[inserted]){
        //Swap with parent
        swap_with_parent(inserted);
        //New index to test is at parent index since they swapped
        inserted=parent;
        //Get new parent index of inserted
        parent=parent_index(inserted);

    }

}

template <typename T>
T Heap<T>::Pop(){
    /**
       * @brief DEBUG
       * function:Pop
       * purpose: Pop root of tree
       * Replace it with last inserted item
       * Have the item sink to its position
       * parameters:
       */
    //Assert if popping an empty tree
    if(is_empty()){
        assert(false);
    }

    int bigger_child;
    int root=0;
    //Grab item
    T item=_tree[root];

    //Set top of tree = to last element
    _tree[root]=_tree[how_many-1];

    //Take out item at last element
    _tree.pop_back();

    //Reduce size
    how_many=_tree.size();

    //If the tree is not empty after popping, last element
    if(!is_empty()){
        //While root is not a leaf
        while(!is_leaf(root)){
            //Find bigger child
            bigger_child=big_child_index(root);

            //Do swapping/sinking
            //If root is less than bigger child
            if(_tree[root]<_tree[bigger_child]){
                //Swap bigger child with his parent(root)
                swap_with_parent(bigger_child);

                //Now root is at the bigger child index
                root=bigger_child;
            }
            //If not Leave loop
            else{
                break;
            }
        }

    }

    //Return item popped
    return item;
}

template <typename T>
void Heap<T>::clear(){
    /**
       * @brief DEBUG
       * function:clear
       * purpose: clear vector
       * Set _tree= empty vector
       * How_many=0;
       * Capacity=1;
       * parameters: none
       */
    Vector<T> empty;

    _tree=empty;

    how_many=_tree.size();

}

template <typename T>
int Heap<T>::size() const{
    /**
       * @brief DEBUG
       * function:size
       * purpose: return how_many
       * parameters: none
       */
    return how_many;
}

template <typename T>
int Heap<T>::capacity() const{
    /**
       * @brief DEBUG
       * function: capacity
       * purpose: return _tree.capacity()
       * parameters: none
       */
    return _tree.capacity();
}

template <typename T>
bool Heap<T>::is_empty() const{
    /**
       * @brief DEBUG
       * function:is empty
       * purpose: return true if empty
       * return false if not
       * return _tree.empty();
       * parameters: none
       */
    return _tree.empty();
}


template <typename T>
void Heap<T>::print_tree(ostream& outs) const{
    /**
       * @brief DEBUG
       * function:print_tree
       * purpose:if tree is not empty, print tree as an array
       * parameters: ostream& outs
       */
    if(is_empty()!=true){
        for(int i=0;i<how_many;i++){
            cout<<_tree[i]<<" ";
        }

    }


}

template <typename T>
void Heap<T>::print_tree(unsigned int root,int level,ostream& outs) const{
    /**
       * @brief DEBUG
       * function:print_tree
       * purpose: Print Heap as a tree
       * if it isn't empty print Heap as a tree
       * parameters:unsigned int root, int level, ostream& outs
       */

    //If it isn't empty
    if(is_empty()!=true){
        //Continue printing as long as root is in range of size
        if(root<=how_many-1){
            //Go right
            print_tree(right_child_index(root),level+1,outs);

            //Print
            cout<<setw(level*4)<<""<<"["<<_tree[root]<<"]"<<endl;

            //Go left
            print_tree(left_child_index(root),level+1,outs);

        }

    }

}

template <typename T>
bool Heap<T>::is_leaf(int i) const{
    /**
       * @brief DEBUG
       * function:
       * purpose:
       * parameters:
       */
    unsigned int left_child=left_child_index(i);
    unsigned int right_child=right_child_index(i);
    unsigned int range=how_many-1;

    //If left child&&right child out of range, they don't exist
    if((left_child>range)&&(right_child>range)){
        return true;
    }
    //One of them or more are in range, they exist, so not a leaf
    else{
        return false;
    }


}

template <typename T>
unsigned int Heap<T>::parent_index(unsigned int i) const{
    /**
       * @brief DEBUG
       * function:Parent_index
       * purpose: Return i's parent's index
       * i-1/2
       * parameters:unsigned int i
       */
    //If i is 0, return 0, can't go negative in unsigned int
    if(i==0){
        return 0;
    }
    //Return (i-1)/2
    return (i-1)/2;
}

template <typename T>
unsigned int Heap<T>::left_child_index(unsigned int i) const{
    /**
       * @brief DEBUG
       * function:left_child index
       * purpose: Return left child index of i
       * return (2*i)+1
       * parameters:unsigned int i
       */
    //Return (2*i)+1
    return((2*i)+1);
}

template <typename T>
unsigned int Heap<T>::right_child_index(unsigned int i) const{
    /**
       * @brief DEBUG
       * function:right_child_index
       * purpose: return right child index of i
       * Return (2*i)+2
       * parameters:unsigned int
       */
    return ((2*i)+2);
}

template <typename T>
unsigned int Heap<T>::big_child_index(unsigned int i) const{
    /**
       * @brief DEBUG
       * function:Big Child Index
       * purpose: Return i's bigger child index
       * parameters:unsigned int i
       */
    //Get right child and left child
    unsigned int right_index=right_child_index(i);
    unsigned int left_index=left_child_index(i);

    //Range is size-1
    unsigned int range=how_many-1;

    //If right index is out of range,left is automatically bigger
    if(right_index>range){
        return left_index;
    }

    //If left is smaller than right, return right
    if(_tree[left_index]<_tree[right_index]){
        return right_index;
    }
    //Other wise, return left
    else{
        return left_index;
    }


}

template <typename T>
void Heap<T>::swap_with_parent(unsigned int i){
    /**
       * @brief DEBUG
       * function:swap with parent
       * purpose: swap i with its parent's position
       * parameters:unsigned int i
       */
    //Get parent index
    unsigned int parent=parent_index(i);

    //Save parent item
    T item=_tree[parent];

    //Parent=Child
    _tree[parent]=_tree[i];

    //Child=Parent Item
    _tree[i]=item;

}

#endif // HEAP_CLASS_H
