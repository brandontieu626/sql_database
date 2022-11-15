#ifndef BPTMAP_CLASS_H
#define BPTMAP_CLASS_H
#include "../../!includes/bplustree_class/bplustree.h "
#include "../../!includes/pair_struc/pair_struc.h"
template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K,V> > map_base;

    class Iterator{
    public:
        friend class Map;
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

        Pair<K, V> operator *(){
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

    Map(bool dups=false);

    //Iterators
    Iterator begin();  //Return iterator to left most node
    Iterator end();    //Return iterator to NULL

    int size() const;   //Return total data count of tree
    bool empty() const; //Return if tree is empty

    V& operator[](const K& key);  //Return V& for Pair with Key
    V& at(const K& key);          //Return V& for Pair with Key
    const V& at(const K& key) const; //Return const V& for Pair with key

    void insert(const K& k, const V& v);  //Insert Pair(k,v)
    void erase(const K& key);         //Remove Pair with key k
    void clear();                     //Clear entire tree
    V get(const K& key);              //Return v for Pair with key k

    Iterator find(const K& key);      //Return iterator to node with key
    bool contains(const Pair<K, V>& target) const;

    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }

private:
    int key_count;              //Total keys in map
    BPlusTree<Pair<K,V> > map;

};

template <typename K, typename V>
Map<K,V>::Map(bool dups):key_count(0),map(dups){
    /**
       * @brief DEBUG
       * function:Map Default CTOR
       * purpose:Set Key_count=0;
       * parameters:none
       */

}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin(){
    /**
       * @brief DEBUG
       * function:Begin
       * purpose:Return iterator of map's iterator of smallest node
       * parameters:none
       */
    return Iterator(map.begin());
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end(){
    /**
       * @brief DEBUG
       * function:end
       * purpose:Return iterator of map's iterator of NULL
       * parameters:none
       */
    return Iterator(map.end());
}

template <typename K, typename V>
int Map<K,V>::size() const{
    /**
       * @brief DEBUG
       * function:size
       * purpose:return map.size
       * parameters:none
       */
    return map.size();
}

template <typename K, typename V>
bool Map<K,V>::empty() const{
    /**
       * @brief DEBUG
       * function:empty
       * purpose:return map.empty()
       * parameters:none
       */
    return map.empty();
}

template <typename K, typename V>
V& Map<K,V>::operator[](const K& key){
    /**
       * @brief DEBUG
       * function:subscript operator
       * purpose:return reference of item with key
       * parameters:const K& key
       */

//    cout<<"map["<<key<<"]";
    return map.get(key).value;
}

template <typename K, typename V>
V& Map<K,V>::at(const K& key){
    /**
       * @brief DEBUG
       * function:at
       * purpose:return reference of item with key
       * parameters:const K& key
       */

//    cout<<"map.at("<<key<<")";
    return map.get(key).value;

}

template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const{
    /**
       * @brief DEBUG
       * function:at
       * purpose:return const reference of item with key
       * parameters:const K& key
       */
//    cout<<"CONSTANT AT VERSIOn"<<endl;
    return map.get_existing(key).value;
}

template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v){
    /**
       * @brief DEBUG
       * function:insert
       * purpose:insert Pair k,v
       * call map.insert
       * parameters:const K& k, const V& v
       */
    Pair<K,V> add(k,v);  //Create Object

//    cout<<"Inserting: "<<add<<endl;
    //If added new node,increase key count
    if(map.insert(add)){
        key_count++;
    }
    else{

    }
}

template <typename K, typename V>
void Map<K,V>::erase(const K& key){
    /**
       * @brief DEBUG
       * function:erase
       * purpose:Remove object with key from tree
       * Call map.remove
       * parameters:const K& key
       */

//    cout<<"Erasing: "<<key<<endl;
    //If removed successfully, decrease key count
    if(map.remove(key)){
        key_count--;
//        cout<<"REMOVED SUCCESSFULLY: "<<key_count<<endl;
    }
    else{
//        cout<<"NO REMOVAL: "<<key_count<<endl;
    }

}

template <typename K, typename V>
void Map<K,V>:: clear(){
    /**
       * @brief DEBUG
       * function:clear
       * purpose:clear the entire tree
       * call map.cleartree
       * parameters:none
       */
    map.clear_tree();
}

template <typename K, typename V>
V Map<K,V>::get(const K& key){
    /**
       * @brief DEBUG
       * function:get
       * purpose:get item with key, return value in this
       * parameters:const K& key
       */

    return map.get(key).value;

}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K& key){
    /**
       * @brief DEBUG
       * function:find
       * purpose:Return iterator of node with key
       * parameters:const K& key
       */


    return Iterator(map.find(key));
}



template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target) const{
    /**
       * @brief DEBUG
       * function:contains
       * purpose:
       * return bool
       * True if it contains/False otherwise
       * parameters:const Pair<K, V>& target
       */

    return map.contains(target);

}

template <typename K,typename V>
typename Map<K,V>::Iterator Map<K,V>::lower_bound(const K& key){
    /**
       * @brief DEBUG
       * function:Lower bound
       * purpose:Return Iterator of Lowerbound from Map
       * parameters:const K& key
       */
    return Iterator (map.lower_bound(key));
}

template <typename K,typename V>
typename Map<K,V>::Iterator Map<K,V>::upper_bound(const K& key){
    /**
       * @brief DEBUG
       * function:Upper Bound
       * purpose:Return Iterator of Upperbound from Map
       * parameters:const K& key
       */

    return Iterator (map.upper_bound(key));
}






#endif // BPTMAP_CLASS_H
