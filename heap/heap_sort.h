#ifndef HEAP_SORT_H
#define HEAP_SORT_H
#include "../../!includes/heap_priority_queue_class/heap_class.h"

template <typename T>
void sort(Vector<T>& sorted){
    /**
       * @brief DEBUG
       * function:sort
       * purpose:Heap Sort of a vector
       * parameters:Vector<T>& sorted
       */
    unsigned int sorting=sorted.size()-1;

    Heap<T> heapify;

    //Add everything to heap
    for(int i=0;i<sorted.size();i++){
        heapify.insert(sorted[i]);
    }

    //Pop everything out and insert back into Vector
    while(!heapify.is_empty()){
        sorted[sorting]=heapify.Pop();

        sorting--;
    }
}

#endif // HEAP_SORT_H
