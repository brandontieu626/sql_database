#ifndef ADD_ENTRY_INCLUDES_FUNCTIONS_H
#define ADD_ENTRY_INCLUDES_FUNCTIONS_H
#include <iostream>

using namespace std;

//Prototype/Signature
template<typename T>
T* find(T item, T* array, int size);

template<typename T>
int search(const T& item, T* array, int size);

template<typename T>
void print_array(T* a,int size);

template<typename T>
void init_array(T* a, int size,T item);

template<typename T>
void copy(T dest[],T src[],int size);

template<typename T>
T* allocate(int capacity);

template<typename T>
T* add_entry(T* list, const T& new_entry,int& size, int& capacity);

template<typename T>
void shift_left(T* a,T* mark,int& size);

template<typename T>
void shift_right(T* a,T* mark,int& size);

template<typename T>
void append(T item, T* array,int& size);

template<typename T>
T* remove_entry(T* list, const T& delete_me,int& size, int& capacity);

template<typename T>
T* erase_entry(T* array,int &size,int& capacity,int mark);



//Definitions
template<typename T>
void append(T item, T* array,int& size){
    /**
     * @brief DEBUG
     * function: Appends item to end of array
     * purpose: Append item to end of array
     * Increment Size
     * parameters: T item, T* array/pointer, int size
     */

    //Append to next empty element
    array=array+size;

    //Set element = item
    *array=item;

    //Increment
    size++;

}

template<typename T>
void shift_right(T* a,T* mark,int& size){
    /**
     * @brief DEBUG
     * function: Shift Right
     * purpose: shift right at mark
     * parameters: T* array, T* mark, int size
     */
    T* a_walker;
    T* end_of_list;
    end_of_list=a+(size-1);


    for(;end_of_list>=mark;end_of_list--){
        a_walker=end_of_list+1;
        *a_walker=*end_of_list;
    }

}

template<typename T>
void shift_left(T* a,T* mark,int& size){
    /**
     * @brief DEBUG
     * function: Shift Left
     * purpose: Shift left at mark
     * parameters: T* a, T* mark, int size
     */
    T* a_walker;
    T* end_of_list;
    end_of_list=a+(size-1);

    for(;mark<end_of_list;mark++){
        a_walker=mark+1;
        *mark=*a_walker;
    }
    size--;


}

template<typename T>
T* remove_entry(T* list, const T& delete_me,int& size, int& capacity){
    /**
     * @brief DEBUG
     * function: Remove entry
     * purpose: Search for item and shift left from it
     * Reallocate if size is <= 1/4 of capacity
     * Return old or new list
     * parameters: T* array, T delete_me, int size, int capacity
     */
    T* remove_item_pointer;

    T* new_list;

    remove_item_pointer=find(delete_me,list,size);


    if(remove_item_pointer!=NULL){
        shift_left(list,remove_item_pointer,size);
    }

    if(size<=(capacity/4)){
        capacity=capacity/2;
        if(capacity==0){
            capacity=1;
        }
        new_list=allocate<T>(capacity);
        copy(new_list,list,size);
//        cout<<"Delete List Before: "<<*list;
        delete []list;                         //Delete Previous Array
//        cout<<"Delete List After: "<<*list;
        return new_list;                       //Return New Array
    }

    return list;                               //Return Same Array


}

template<typename T>
T* add_entry(T* list, const T& new_entry,int& size, int& capacity){
    /**
     * @brief DEBUG
     * function: Add Entry, Add an item to end of list
     * purpose:
     * Reallocate if size is <= 1/4 of capacity
     * Return original list or new list
     * parameters: T* array, T delete_me, int size, int capacity
     */
    T* new_list;


    if(size==capacity){
        capacity=capacity*2;

        new_list=allocate<T>(capacity);

        copy(new_list,list,size);

        append(new_entry,new_list,size);

        delete[]list;                      //Delete Original Array after copying

        return new_list;                   //Return new array
    }
    else{
        append(new_entry,list,size);

        return list;                      //Return same array
    }


}

template<typename T>
T* allocate(int capacity){
    /**
     * @brief DEBUG
     * function: Allocate space
     * purpose: Returns pointer to new[capacity]
     * parameters: int capacity
     */
    const bool debug=false;

    if(debug){
        cout<<endl<<"Allocate: Capacity: "<<capacity;
    }


    return new T[capacity];
}

template<typename T>
void print_array(T* a,int size){
    /**
     * @brief DEBUG
     * function: print array
     * purpose: Print an array
     * parameters: T* array, int size
     */
    for(int i=0;i<size;i++){
        cout<<*a<<" ";
        a++;
    }
}

template <typename T>
void init_array(T* a, int size,T item){
    /**
     * @brief DEBUG
     * function: initialize an array
     * purpose: initialize array to item
     * parameters:T* array, int size, T item
     */
    for(int i=0;i<size;i++){
        *a=item;
        a++;
    }
}

template<typename T>
T* find(T item, T* array, int size){
    /**
     * @brief DEBUG
     * function: Find an item in array
     * purpose: Return pointer to that array
     * if not found return NULL
     * parameters:T item, T* array, int size
     */
    for(T* w=array;w-array<size;w++){
        if(*w==item){
            return w;
        }
    }

    return NULL;

}

template<typename T>
int search(const T& item, T* array, int size) {
    /**
     * @brief DEBUG
     * function: Search for item in array
     * purpose: Return index of item in array
     * If not found return -1
     * parameters:T item, T* array, int size
     */
    int index=0;

    for(T* w=array;w-array<size;w++){
        if(*w==item){
            return index;
        }
        index++;
    }



    return -1;


}

template<typename T>
void copy(T dest[],T src[],int size){
    /**
     * @brief DEBUG
     * function: Copy Src to Dest Array
     * purpose:
     * parameters:T item, T* array, int size
     */
    T* walker_dest;
    T* walker_src;

    walker_dest=dest;
    walker_src=src;

    for(int i=0;i<size;i++){
        *walker_dest=*walker_src; //Dereference both dest_walker and src_walker,
                                  //set address that dest_walker points to equal to address walker points to.
        walker_dest++;            //Increment dest walker address
        walker_src++;             //Increment src walker address
    }
}

template<typename T>
T* erase_entry(T* array,int &size,int& capacity,int mark){
    /**
     * @brief DEBUG
     * function: Remove item at position
     * purpose: Remove item at position, shift left at position
     * Allocate new space if size <=1/4 of capacity
     * Return pointer to original list or new list
     * parameters:T* array, int size, int capacity,int mark
     */
    T* new_list;
    T* mark_walker=array;

    mark_walker+=mark;

    shift_left(array,mark_walker,size);

    if(size<=(capacity/4)){
        capacity=capacity/2;
        if(capacity==0){
            capacity=1;
        }
        new_list=allocate<T>(capacity);
        copy(new_list,array,size);
        delete []array;                         //Delete Previous Array
        return new_list;                       //Return New Array
    }

    return array;                               //Return Same Array

}




#endif // ADD_ENTRY_INCLUDES_FUNCTIONS_H
