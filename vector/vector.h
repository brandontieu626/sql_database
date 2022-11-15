#ifndef VECTOR_H
#define VECTOR_H
#include "../../../CS3A/!includes/add_entry_includes_functions/add_entry_includes_functions.h"
#include "../../!includes/bptmap_class/bptmap_class.h"
#include <assert.h>
template <class T>
class Vector
{
public:
    Vector();                               //Default Constructor
    Vector(int capacity);
    Vector(T arr[],int size);
    Vector(initializer_list<T> list);
    //Big 3 Functions
    Vector(const Vector &other);            //Copy Constructor
    ~Vector();                              //Destructor
    Vector& operator =(const Vector &rhs);  //Assignment

    //Member Access Functions:
    const T& operator [](const int index) const;
    T& operator[](const int index);
    T& at(int index);                   //return reference to item at position index
    const T& at(int index) const;       //return a const item at position index
    T& front() const;                   //return item at position 0
    T& back() const;                    //return item at last position

    //Push and Pop Functions
    Vector& operator +=(const T& item);  //push_back
    void push_back(const T& item);            //Append item to end of vector
    T pop_back();                        //Remove item at end of vector and return it

    Vector<T> pick(Vector<int> list);

    //Overloaded Friend Operators
    template <typename U>
    friend ostream& operator <<(ostream& outs,const Vector<U>& print_me);
    template <typename U>
    friend  Vector<U> operator +(const Vector<U>& vector_1,const Vector<U>& vector_2);

    //Insert and Erase
    void insert(int insert_here, const T& insert_this); //insert at pos
    void erase(int index);                              //erase at pos
    int index_of(const T& item);                        //return index of item

    //Size and Capacity
    void set_capacity(int capacity);                    //Allocate space
    int size() const;
    int capacity() const;

    bool empty() const;




private:
    int _size;
    int _capacity;
    T* _v;

};


template <typename T>
Vector<T> union_m(Vector<T>& first,Vector<T>& second){
    /**
       * @brief DEBUG
       * function:
       * purpose:
       * parameters:
       */
    Vector<T> unioned;

    int i=0;
    int j=0;

    while(i<first.size()||j<second.size()){
//        cout<<"i:"<<i<<endl;
//        cout<<"First Size:"<<first.size()<<endl;
        if(i==first.size()){
            while(j<second.size()){
                unioned+=second[j];
                j++;
            }
            break;
        }
        else if(j==second.size()){
            while(i<first.size()){
                unioned+=first[i];
                i++;
            }
            break;
        }

        if(first[i]<second[j]){
            unioned+=first[i];
            i++;
        }
        else if(first[i]>second[j]){
            unioned+=second[j];
            j++;
        }
        else{
            unioned+=first[i];
            i++;
            j++;
        }
    }

    return unioned;


}

template <typename T>
Vector<T> intersection_m(Vector<T>& first,Vector<T>& second){
    /**
       * @brief DEBUG
       * function:
       * purpose:
       * parameters:
       */
    Vector<T> intersected;
    int i=0;
    int j=0;

    while(i<first.size()&&j<second.size()){
        if(first[i]>second[j]){
            j++;
        }
        else if(first[i]<second[j]){
            i++;
        }
        else{
            intersected+=first[i];
            i++;
            j++;
        }
    }

    return intersected;

}

//Definitions
template <typename T>
Vector<T>::Vector(){
    /**
     * @brief DEBUG
     * function: Default CTOR for Vector Object
     * purpose: Sets _capacity=1;
     * _size=0;
     * parameters: int capacity
     */
     const int DEBUG=false;
    _capacity=1;
    _size=0;
    //Allocate space for vector
    _v=allocate<T>(_capacity);
    if(DEBUG){
        cout<<"Default CTOR"<<endl;
    }
}
template <typename T>
Vector<T>::Vector(int capacity)
{
    /**
     * @brief DEBUG
     * function: CTOR for Vector (Capacity argument)
     * purpose: Sets _capacity to argument int capacity
     * parameters: int capacity
     */
    const int DEBUG=false;

    //Set _capacity=capacity
    _capacity=capacity;
    _size=0;

    //Allocate space
    _v=allocate<T>(_capacity);
    if(DEBUG){
        cout<<"CTOR"<<endl;
    }

}

template <typename T>
Vector<T>::Vector(T arr[],int size)
{
    /**
     * @brief DEBUG
     * function: CTOR for Vector (Capacity argument)
     * purpose: Sets _capacity to argument int capacity
     * parameters: int capacity
     */
    const int DEBUG=false;
    _capacity=1;
    _size=0;

    //Allocate space for vector
    _v=allocate<T>(_capacity);

    for(int i=0;i<size;i++){
        push_back(arr[i]);
    }

    if(DEBUG){
        cout<<"ARR CTOR"<<endl;
        cout<<"CAPACITY: "<<_capacity<<endl;
        cout<<"SIZE: "<<_size<<endl;
    }

}

template <typename T>
Vector<T>::Vector(initializer_list<T> list)
{
    /**
     * @brief DEBUG
     * function: CTOR for Vector (Capacity argument)
     * purpose: Sets _capacity to argument int capacity
     * parameters: int capacity
     */
    const int DEBUG=false;
    _capacity=1;
    _size=0;

    //Allocate space for vector
    _v=allocate<T>(_capacity);

    for(T element:list){
        push_back(element);
    }

    if(DEBUG){
        cout<<"ARR CTOR"<<endl;
        cout<<"CAPACITY: "<<_capacity<<endl;
        cout<<"SIZE: "<<_size<<endl;
    }

}

template <typename T>
Vector<T>:: Vector(const Vector &other){
    /**
     * @brief DEBUG
     * function: Copy CTOR
     * purpose: Copy Vector other into CTOR Vector
     * set _capacity to other capacity
     * set _size to other size
     * create new space of _capacity
     * Copy elements of other into CTOR Vector
     * parameters: Vector object_to_be_copied
     */
    const int DEBUG=false;
    //Set _capacity and _size= to other
    _capacity=other._capacity;
    _size=other._size;

    //Allocate Space
    _v=allocate<T>(_capacity);

    //Copy Elements from other into _v
    copy(_v,other._v,_size);

    if(DEBUG){
        cout<<"Copy CTOR"<<endl;
    }

}

template <typename T>
Vector<T>::~Vector(){
    /**
     * @brief DEBUG
     * function: Destructor function for vector
     * purpose: deletes the space for each vector object
     * Deletes the space that is pointed to by private member _v.
     * Automatically called once object is out of scope
     * parameters: none
     */
    const int DEBUG=false;

    //Delete space
    delete [] _v;

    if(DEBUG){
        cout<<"~Vector()"<<endl;
    }
}

template <typename T>
Vector<T>& Vector<T>::operator =(const Vector<T> &rhs){
    /**
     * @brief DEBUG
     * function: Overloaded assignment operator
     * purpose: Set set Left Vector=Right Vector.
     * Set size and capacity =right vector size and capacity
     * Delete Original space
     * Allocate new space with new capacity
     * Copy Right Vector elements to new space
     * Return itself
     * parameters: Vector right
     */
    const int DEBUG=false;


    //Self Reference
    if(this==&rhs){
        return *this;
    }


    //Set size and capacity
    _size=rhs._size;
    _capacity=rhs._capacity;
    //Delete Original Space
    delete []_v;

    //Point at new space
    _v=allocate<T>(_capacity);

    //Copy over rhs
    copy(_v,rhs._v,_size);

    if(DEBUG){
        cout<<"Assigment Operator="<<endl;
    }
    return *this;

}

template <typename T>
const T& Vector<T>:: operator [](const int index) const{
    /**
     * @brief DEBUG
     * function: Overloaded subscript operator (Const Version)
     * purpose: Returns the element at index using subscript
     * Returns a const T so it can NOT be used in assignment
     * such as Vector[2]=10;
     * parameters: index
     */

    const int DEBUG=false;
    if(DEBUG){
        cout<<"Const Subscript Called"<<endl;
    }
    //Create a walker to move _v
    T* vector_walker=_v;

    //Point to item
    vector_walker+=index;

    //Dereference and return item
    return *vector_walker;


}

template <typename T>
T& Vector<T>:: operator[](const int index){
    /**
     * @brief DEBUG
     * function: Overloaded subscript operator (Non-Const Version)
     * purpose: Returns the element at index using subscript
     * Returns address of T so it CAN be used in operations like:
     * v[i]=25;
     * parameters: int index
     */
    const int DEBUG=false;
    if(DEBUG){
        cout<<"Non-Const Subscript Called"<<endl;
    }

    //Create a walker to move _v
    T* vector_walker=_v;

    //Point to item
    vector_walker+=index;

    //Dereference and return item
    return *vector_walker;

}
template <typename T>
T& Vector<T>:: at(int index){
    /**
     * @brief DEBUG
     * function: Returns element at index location
     * purpose: Returns the element at index using function call
     * Returns address of T so it CAN be used in operations like:
     * v[i]=25;
     * parameters: int index
     */

    //Create walker to move _v
    T* vector_walker;

    vector_walker=_v;

    //Point to item
    vector_walker+=index;

    //Dereference and return item
    return *vector_walker;


}
template <typename T>
const T& Vector<T>:: at(int index) const{
    /**
     * @brief DEBUG
     * function: Returns element at index location (Const)
     * purpose: Returns the element at index using function call
     * Returns address of T so it can't be used in operations like:
     * v[i]=25;
     * parameters: int index
     */

    //Create walker to move _v
    T* vector_walker;
    vector_walker=_v;

    //Point to item
    vector_walker+=index;

    //Dereference and return item
    return *vector_walker;

}
template <typename T>
T& Vector<T>:: front() const{
    /**
     * @brief DEBUG
     * function: Returns element at front of vector
     * purpose: Returns element at index 0
     * Checks if empty, if it is return NULL
     * parameters: none
     */

    //Assert if it is empty
    assert(empty()==false);

    //Dereference and return item in front
    return *_v;






}
template <typename T>
T& Vector<T>::back() const{
    /**
     * @brief DEBUG
     * function: Returns element at front of vector
     * purpose: Returns element at index 0
     * Checks if empty, if it is return NULL
     * parameters: none
     */

    //Assert if it is empty
    assert(empty()==false);


    //Create walker to move _v
    T* vector_walker=_v;

    //Point to end of array
    vector_walker+=_size-1;

    //Dereference and return item
    return *vector_walker;


}
template <typename T>
Vector<T>& Vector<T>:: operator +=(const T& item){
    /**
     * @brief DEBUG
     * function: Overloaded += operator
     * purpose: Allow for easy addition of elements to vector
     * adds item to vector
     * parameters: int item
     */

    //Call push back to add to end of vector
    push_back(item);

    //Return itself
    return *this;
}

template <typename T>
void Vector<T>::push_back(const T& item){
    /**
     * @brief DEBUG
     * function: Add item to end of vector
     * purpose: Add item to vector through add_entry
     * Allocates double the space every time size >capacity
     * Increments size
     * parameters: int item
     */

    const int DEBUG=false;
    //Call add entry to add to end of list
    //This handles all reallocation of space
    _v=add_entry(_v,item,_size,_capacity);
    if(DEBUG){
         cout<<"PUSH BACK Size: "<<_size<<endl;
         cout<<"PUSH BACK Capacity: "<<_capacity<<endl;

    }

}

template <typename T>
T Vector<T>::pop_back(){
    /**
     * @brief DEBUG
     * function: Remove item at end of vector and return the element removed
     * purpose: Remove item at end of vector using shift left
     * parameters: none
     */
    const int DEBUG=false;

    //Pointer that points to last element
    T* end=_v+(_size-1);
    T last_element=*end;

    //If empty return NULL
    if(empty()==true){
        assert(false);
    }
    else{
        //Erase entry handles all reallocation of capacity
       _v=erase_entry(_v,_size,_capacity,_size-1);

    }




    if(DEBUG){
        cout<<"POP BACK Capacity: "<<_capacity<<endl;
        cout<<"POP Back Size: "<<_size<<endl;
    }

    return last_element;



}

template <typename T>
Vector<T> Vector<T>::pick(Vector<int> list){
    /**
       * @brief DEBUG
       * function:
       * purpose:
       * parameters:
       */
    Vector other;


    for(int i=0;i<list.size();i++){
        other.push_back(at(list[i]));
    }

    return other;
}


template <typename U>
ostream& operator <<(ostream& outs,const Vector<U>& print_me){
    /**
     * @brief DEBUG
     * function: Overloaded insertion operator
     * purpose: Easy printing for Vectors
     * parameters: ostream& outs, Vector print_me
     */

    //Call Print array for printing
    print_array(print_me._v,print_me._size);


    return outs;
}

template <typename U>
Vector<U> operator +(const Vector<U> &vector_1, const Vector<U>  &vector_2){
    /**
     * @brief DEBUG
     * function: Overloaded + operator
     * purpose: Allow for easy addition of two vector objects
     * returns Third Vector Object of both vectors combined.
     * parameters: Vector object1, Vector object2
     */

    //Add the sizes and capacity for new vector
    int new_size=(vector_1._size+vector_2._size);
    int new_capacity=(vector_1._capacity+vector_2._capacity);

    //Construct new vector object
    Vector<U> third_vector(new_capacity);

    //Copy first vector elements
    copy(third_vector._v,vector_1._v,vector_1._size);

    //Use walker so it is not effected when returned
    U* third_vector_walker=third_vector._v;

    //Start at last element copied
    third_vector_walker+=vector_1._size;

    //Copy second vector elements
    copy(third_vector_walker,vector_2._v,vector_2._size);

    //Set size to new size
    third_vector._size=new_size;

    //Return third vector
    return third_vector;






}

template <typename T>
void Vector<T>:: insert(int insert_here, const T& insert_this){
    const int DEBUG=false;
    //Pointer to the index
    T* mark_pointer=_v+insert_here;

    //Pointer to end of size/interesting elements
    T* size_walker=_v+(_size-1);


    //Assert if the mark is outside of size/interesting elements
    if(empty()==false){
        assert(mark_pointer<=size_walker);
    }


    //Push back to allocate space if needed
    push_back(insert_this);

    //Pop back the element
    pop_back();

    //Set pointer to index
    mark_pointer=_v;
    mark_pointer+=insert_here;

    //Shift right at index
    shift_right(_v,mark_pointer,_size);

    //Insert element into index
    at(insert_here)=insert_this;

    //Increment size
    _size++;

    if(DEBUG){
        cout<<"INSERT Size: "<<_size<<endl;
        cout<<"INSERT Capacity: "<<_capacity<<endl;
    }



}

template <typename T>
void Vector<T>::erase(int index){
    /**
     * @brief DEBUG
     * function: erase element at index
     * purpose: shift left at index
     * parameters: int index
     */

    const int DEBUG=false;

    //Pointer to the index
    T* mark_pointer=_v+index;
    //Pointer to end of size/interesting elements
    T* size_walker=_v+(_size-1);

    //Assert if the mark is outside of size/interesting elements
    assert(mark_pointer<=size_walker);

    //If not empty,allow erasing
    if(empty()==false){
        _v=erase_entry(_v,_size,_capacity,index);
    }


    if(DEBUG){
        cout<<"ERASE Size: "<<_size<<endl;
        cout<<"ERASE Capacity: "<<_capacity<<endl;
    }

}

template <typename T>
int Vector<T>::index_of(const T& item){
    /**
     * @brief DEBUG
     * function: Returns the index of an item
     * purpose: Searchs vector for item and returns index of element
     * If not found, return -1
     * parameters: int item
     */

    //Search returns index of item
    return search(item,_v,_size);

}

template <typename T>
void Vector<T>::set_capacity(int capacity){
    /**
     * @brief DEBUG
     * function: Change the capacity of a vector
     * purpose: set the capacity of a vector to int capacity
     * Allocate new space with int capacity
     * Copy over old elements to new space
     * Delete original space
     * set _capacity=capacity and point _v to new space
     * parameters: int capacity
     */
    const int DEBUG=false;

    if(DEBUG){
        cout<<"Old Capacity Pointer: "<<_v<<endl;
        cout<<"Old Capacity: Element: "<<*_v<<endl;
        cout<<"Old Capacity: "<<_capacity<<endl;

    }

    //Allocate New Space
    T* new_capacity=allocate<T>(capacity);

    //Copy over into new space
    copy(new_capacity,_v,_size);

    //Delete Original space
    delete []_v;

    if(DEBUG){
         cout<<"Old Capacity: Element after delete "<<*_v<<endl;
    }


    //Set pointer to new space
    //Set _capacity
    _capacity=capacity;
    _v=new_capacity;

    if(DEBUG){
        cout<<"New Capacity Pointer: "<<_v<<endl;
        cout<<"New Capacity: "<<_capacity<<endl;
        cout<<"New Capacity: Element: "<<*_v<<endl;

    }

}

template <typename T>
int Vector<T>:: size() const{
    /**
     * @brief DEBUG
     * function: Accessor function for size
     * purpose: Returns _size
     * parameters: none
     */
    return _size;
}

template <typename T>
int Vector<T>:: capacity() const{
    /**
     * @brief DEBUG
     * function: Return capacity
     * purpose: accessor function for capacity
     * parameters: none
     */

    //Return capacity
    return _capacity;
}

template <typename T>
bool Vector<T>::empty() const{
    /**
     * @brief DEBUG
     * function: checks if vector is empty
     * purpose: If size==0, return true for empty
     * return false other wise
     * parameters: none
     */
    //Return true if empty
    if(_size==0){
//        cout<<"SIZE IS ZERO!!!"<<endl;
        return true;
    }

    //Return false if not
    return false;
}






#endif // VECTOR_H
