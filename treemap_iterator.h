// C++ Aufgabe tree_map von Hartmut Schirmacher, BHT Berlin

#pragma once

// class definitions
#include "treemap_class.h"

namespace my {

    // iterator: references a node within the tree
    template<typename K, typename T>
    class treemap<K,T>::iterator {
    protected:

        // treemap is a friend, can call protected constructor
        friend class treemap;

        // construct iterator referencing a specific node
        // - only treemap shall be allowed to do so
        iterator(std::weak_ptr<treemap<K, T>::node> node)
            : nodePtr_(node)
        {}

        // non-owning reference to the actual node
        std::weak_ptr<node> nodePtr_;
        // std::weak_ptr<node> iterRoot_;
        // bool isEnd_;

    public:

        // access data of referenced map element (node)
        value_type& operator*()
        {
            /* todo */ static value_type dummy; return dummy;
        }
        value_type* operator->()
        {
            auto access = nodePtr_.lock();
            assert(access);
            return &access->data_;
        }

        // two iterators are equal if they point to the same node
        bool operator==(const iterator&) const
        {
            /* todo */ return false;
        }

        bool operator!=(const iterator&) const
        {
            /* todo */ return false;
        }

        // next element in map, pre-increment
        // note: must modify self!
        iterator& operator++()
        {
            /* todo */ static iterator dummy; return dummy;
        }

        // prev element in map, pre-decrement
        // note: must modify self!
        iterator& operator--()
        {
            /* todo */ static iterator dummy; return dummy;
        }

    }; // class iterator

} // my::

