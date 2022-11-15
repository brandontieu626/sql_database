#ifndef BPTMMAP_CLASS_H
#define BPTMMAP_CLASS_H
#include "../../!includes/bplustree_class/bplustree.h"
#include "../../!includes/mpair_struc/mpair_struc.h"
template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K,V> > map_base;
    class Iterator{
    public:
        friend class MMap;

        Iterator(typename map_base::Iterator it){
            /**
               * @brief DEBUG
               * function:Iterator CTOR
               * purpose:_it=it
               * Set Map Iterator = BPlusTree iterator
               * parameters:typename map_base::Iterator it
               */
            _it=it;
        }

        Iterator operator ++(int unused){
            /**
               * @brief DEBUG
               * function:postfix ++
               * purpose:_it++
               * parameters:int unused
               */
            _it++;

            return *this;
        }

        Iterator operator ++(){
            /**
               * @brief DEBUG
               * function:Prefix ++
               * purpose:++ _it
               * parameters:none
               */
            ++_it;

            return *this;

        }

        MPair<K, V> operator *(){
            /**
               * @brief DEBUG
               * function:Dereference operator
               * purpose:Return *_it
               * parameters:none
               */
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            /**
               * @brief DEBUG
               * function:==
               * purpose:Return lhs._it==rhs._it
               * parameters:const Iterator& lhs, const Iterator& rhs
               */
            return (lhs._it==rhs._it);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            /**
               * @brief DEBUG
               * function:!=
               * purpose:Return lhs._it!=rhs._it
               * parameters:const Iterator& lhs, const Iterator& rhs
               */
            return (lhs._it!=rhs._it);
        }

    private:
        typename map_base::Iterator _it;

    };

    MMap(bool dups=false);

    Iterator begin();  //Iterator of left most node

    Iterator end();    //Iterator of NULL

    int size() const;   //Return total data count in tree
    bool empty() const; //True/False if tree is empty

    const Vector<V>& operator [](const K& key) const;  //Return const Vector<V>& of key
    Vector<V>& operator [](const K& key);              //Return Vector<V>& of key

    void insert(const K& k,const V& v);             //Insert MPair<k,v>
    void erase(const K& key);
    void clear();                                   //Clear entire tree

    bool contains(const K& key) const ;             //Return true/false depending if tree contains key
    Vector<V>& get(const K& key);                   //Return Vector<V>& of key

    Iterator find(const K& key);                //Return iterator of node with key

    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K,V> > mmap;
};

template <typename K,typename V>
MMap<K,V>::MMap(bool dups):mmap(dups){
    /**
       * @brief DEBUG
       * function:MMap CTOR
       * purpose:Create map with duplicates setting
       * parameters:bool dups
       */
    //empty
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::begin(){
    /**
       * @brief DEBUG
       * function:Begin
       * purpose:Return iterator with smallest node
       * parameters:none
       */
    return Iterator(mmap.begin());
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::end(){
    /**
       * @brief DEBUG
       * function:End
       * purpose:Return iterator witn NULL
       * parameters:none
       */
    return Iterator(mmap.end());
}

template <typename K,typename V>
int MMap<K,V>::size() const{
    /**
       * @brief DEBUG
       * function:size
       * purpose:Return data count of mmap
       * parameters:none
       */
    return mmap.size();
}

template <typename K,typename V>
bool MMap<K,V>::empty() const{
    /**
       * @brief DEBUG
       * function:empty
       * purpose:Return mmap.empty
       * parameters:none
       */
    return mmap.empty();
}

template <typename K,typename V>
const Vector<V>& MMap<K,V>:: operator[](const K& key) const{
    /**
       * @brief DEBUG
       * function:Subscript Const
       * purpose:Return const of Vector with key
       * parameters:const K& key
       */

//     cout<<"const mmap["<<key<<"]";
     return mmap.get_existing(key).value_list;

}

template <typename K,typename V>
Vector<V>& MMap<K,V>:: operator[](const K& key){
    /**
       * @brief DEBUG
       * function:Subscript Operator
       * purpose:Return Vector& of Key
       * parameters:const K& key
       */

//     cout<<"mmap["<<key<<"]";
     return mmap.get(key).value_list;

}

template <typename K,typename V>
void MMap<K,V>::insert(const K& k, const V& v){
    /**
       * @brief DEBUG
       * function:Insert
       * purpose:Insert MPair(k,v)
       * parameters:const K& k,const V& v
       */

    //Create MPair
    MPair<K,V> add(k,v);

//    cout<<"Inserting: "<<add<<endl;

    //Insert it
    mmap.insert(add);



}

template <typename K,typename V>
void MMap<K,V>::erase(const K& k){
    /**
       * @brief DEBUG
       * function:erase
       * purpose:remove MPair with key k
       * parameters:const K& k
       */


    mmap.remove(k);



}

template <typename K,typename V>
void MMap<K,V>::clear(){
    /**
       * @brief DEBUG
       * function:Clear
       * purpose:Clear entire tree of MPairs
       * parameters:none
       */
    mmap.clear_tree();
}

template <typename K,typename V>
bool MMap<K,V>::contains(const K& key) const{
    /**
       * @brief DEBUG
       * function:Contains
       * purpose:Return true if tree contains key
       * Return false if it doesn't contain key
       * parameters:const K& key
       */
    return mmap.contains(key);
}

template <typename K,typename V>
Vector<V>& MMap<K,V>::get(const K& key){
    /**
       * @brief DEBUG
       * function:Get
       * purpose:Return Reference of vector with key
       * parameters:const K& key
       */
    return mmap.get(key).value_list;

}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key){
    /**
       * @brief DEBUG
       * function:Find
       * purpose:Return Iterator to node with key
       * parameters:const K& key
       */

    return Iterator(mmap.find(key));
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::lower_bound(const K& key){
    /**
       * @brief DEBUG
       * function:Lower bound
       * purpose:Return Iterator of Lowerbound from Multimap
       * parameters:const K& key
       */

    return Iterator(mmap.lower_bound(key));
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::upper_bound(const K& key){
    /**
       * @brief DEBUG
       * function:Upper Bound
       * purpose:Return Iterator of Upperbound from Multimap
       * parameters:const K& key
       */

    return Iterator(mmap.upper_bound(key));
}








#endif // BPTMMAP_CLASS_H
