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
        iterator(std::shared_ptr<treemap<K, T>::node> node = nullptr)
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
            return nodePtr_.lock()->data_;
        }
        value_type* operator->()
        {
            auto access = nodePtr_.lock();
            assert(access);
            return &access->data_;
        }

        // two iterators are equal if they point to the same node
        bool operator==(const iterator&rhs) const
        {
            auto thisPtr = nodePtr_.lock();
            auto rhsPtr = rhs.nodePtr_.lock();
            return thisPtr == rhsPtr;
        }

        bool operator!=(const iterator&rhs) const
        {
           return !(*this == rhs);
        }

        // next element in map, pre-increment
        // note: must modify self!
        iterator& operator++()
        {
            auto current = nodePtr_.lock();
            while (current != nullptr)
            {
                // Gibt es rechten Knoten?
                if (current->right_ != nullptr)
                {
                    current = current->right_;
                    // Gibt es linken Knoten?
                    while (current->left_ != nullptr)
                    {
                        current = current->left_;
                    }
                    nodePtr_ = std::weak_ptr<node>(current);
                    return *this;
                }
                else // checke parent
                {
                    auto parentPtr = current->parent_.lock();
                    while (parentPtr != nullptr)
                    {
                        // ist parent kleiner als current?
                        if (parentPtr->data_.first < current->data_.first)
                        {
                            parentPtr = parentPtr->parent_.lock();
                        }
                        // wenn parent größer als current
                        else 
                        {
                            nodePtr_ = std::weak_ptr<node>(parentPtr);
                            return *this;
                        }
                    }
                    // Falls es keinen parent gibt und parent daher nullptr ist
                    *this = iterator(nullptr);
                }
            }
            return *this;
        }

        // prev element in map, pre-decrement
        // note: must modify self!
        iterator& operator--()
        {
            /* todo */ static iterator dummy; return dummy;
        }

    }; // class iterator

} // my::

