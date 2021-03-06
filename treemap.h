// C++ Aufgabe tree_map von Hartmut Schirmacher, BHT Berlin

#pragma once

// other includes
#include <memory>
#include <iostream>
#include <utility>
#include <tuple>
#include <cassert>

// class definitions
#include "treemap_class.h"
#include "treemap_iterator.h"
#include "treemap_node.h"

namespace my
{

    template <typename K, typename T>
    treemap<K, T>::treemap()
        : root_(nullptr), size_(size_t())
    {
    }

    template <typename K, typename T>
    void
    treemap<K, T>::clear()
    {
        root_.reset();
        size_ = 0;
    }

    // random read-only access to value by key
    template <typename K, typename T>
    T treemap<K, T>::operator[](const K &key) const
    {
        auto elemFound = find(key);
        if (elemFound != end())
        {
            auto nodePointer = elemFound.nodePtr_.lock();
            return nodePointer->data_.second;
        }
        else
        {
            return T();
        }
    }

    // random write access to value by key
    template <typename K, typename T>
    T &
    treemap<K, T>::operator[](const K &key)
    {
        auto [nodeIterator, wasInserted] = insert(key, T());
        auto access = nodeIterator.nodePtr_.lock();
        return access->data_.second;
    }

    // number of elements in map (nodes in tree)
    template <typename K, typename T>
    size_t
    treemap<K, T>::size() const
    {
        return size_;
    }

    // move ctor
    template <typename K, typename T>
    treemap<K, T>::treemap(treemap<K, T> &&rhs)
        : treemap()
    {
        swap(*this, rhs);
    }
 
    // deep copy ctor
    template <typename K, typename T>
    treemap<K, T>::treemap(const treemap<K, T> &rhs)
        : root_(nullptr), size_(size_t())
    {
        if (rhs.root_ == nullptr)
        {
            return;
        }
        else
        {
            // *this = copyTree(rhs);
            return;
        }
    }

    // treemap<K, T>::copyTree(const treemap<K, T> &rhs) 
    // {
    //     // 
    //     insert(rhs.root_->data_.first, rhs.root_->data_.second);
    //     auto current = root_;
    //     auto rhsCurrent = rhs.root_;

    //     // copy node in node klasse
    //     // neuen node als kopie von sich selber erzeugen
    //     // und methode rekursvi fd??r kinder aufrufen
    //     // 

    // }

    // assignment (move & copy)
    template <typename K, typename T>
    treemap<K, T> &
    treemap<K, T>::operator=(treemap<K, T> rhs)
    {
        swap(*this, rhs);
        return *this;
    }

    // iterator referencing first element (node) in map
    template <typename K, typename T>
    typename treemap<K, T>::iterator
    treemap<K, T>::begin()
    {
        auto current = root_;
        while (current != nullptr)
        {
            if (current->left_ != nullptr)
            {
                current = current->left_;
            }
            else
            {
                return iterator(current);
            }
        }
        return iterator(current);
    }

    // iterator referencing no element (node) in map
    template <typename K, typename T>
    typename treemap<K, T>::iterator
    treemap<K, T>::end() const
    {
        return iterator(nullptr);
    }

    // add a new element into the tree
    // returns:
    // - iterator to element
    // - true if element was inserted; false if key was already in map
    template <typename K, typename T>
    std::pair<typename treemap<K, T>::iterator, bool>
    treemap<K, T>::insert(const K &key, const T &value)
    {
        // if (tree == empty) new root node with value
        if (root_ == nullptr)
        {
            root_ = std::make_shared<node>(std::make_pair(key, value));
            size_++;
            return std::make_pair(iterator(root_), true);
        }
        else
        {
            auto current = root_;
            // std::cout << "OUTPUT: " << typeid(current).name() << std::endl;
            while (current != nullptr)
            {
                if (key < current->data_.first)
                {
                    if (current->left_ != nullptr)
                    {
                        current = current->left_;
                    }
                    else
                    {
                        // current->left_ wird zu shared_pointer mit pair<key, value> und 
                        // current als parent node
                        current->left_ = std::make_shared<node>(std::make_pair(key, value), current);
                        size_++;
                        return std::make_pair(iterator(current->left_), true);
                    }
                }
                else if (key > current->data_.first)
                {
                    if (current->right_ != nullptr)
                    {
                        current = current->right_;
                    }
                    else
                    {
                        // current->right_ wird zu shared_pointer mit pair<key, value> und 
                        // current als parent node
                        current->right_ = std::make_shared<node>(std::make_pair(key, value), current);
                        size_++;
                        return std::make_pair(iterator(current->right_), true);
                    }
                }
                // current->data_.first == key
                else
                {
                    return std::make_pair(iterator(current), false);
                }
            }
            return std::make_pair(iterator(current), false);
        }
    }

    // add a new element into the tree, or overwrite existing element if key already in map
    // returns:
    // - iterator to element
    // - true if element was newly created; false if value for existing key was overwritten
    template <typename K, typename T>
    std::pair<typename treemap<K, T>::iterator, bool>
    treemap<K, T>::insert_or_assign(const K &key, const T &value)
    {
        auto [nodeIterator, wasInserted] = insert(key, value);
        if(wasInserted)
        {
            return std::make_pair(nodeIterator, wasInserted);
        }
        else
        {
            auto current = nodeIterator.nodePtr_.lock();
            // Wert ??berschreiben
            current->data_.second = value;
            return std::make_pair(iterator(current), false);
        }
    }

    // find element with specific key. returns end() if not found.
    template <typename K, typename T>
    typename treemap<K, T>::iterator
    treemap<K, T>::find(const K &key) const
    {
        auto current = root_;
        while (current != nullptr)
        {
            if (key < current->data_.first)
            {
                if (current->left_ != nullptr)
                {   
                    current = current->left_;
                }
                else
                {
                    return end();
                }
            }
            else if (key > current->data_.first)
            {
                if (current->right_ != nullptr)
                {
                    current = current->right_;
                }
                else
                {
                    return end();
                }
            }
            else
            {
                return iterator(current);
            }
        }
        return end();
    }

    // how often is the element contained in the map?
    template <typename K, typename T>
    size_t
    treemap<K, T>::count(const K &key) const
    {
        auto elemFound = find(key);
        // std::cout << typeid(foundElem).name() << std::endl;
        if (elemFound != end())
        {
            return 1;
        }
        return 0;
    }
    // swap is overloaded in global namespace
    // see https://stackoverflow.com/questions/11562/how-to-overload-stdswap
    // (answer by Attention Mozza314)
    template <typename K, typename T>
    void
    treemap<K, T>::swap(treemap<K, T> &lhs, treemap<K, T> &rhs)
    {
        auto temp = lhs.root_;
        lhs.root_ = rhs.root_;
        rhs.root_ = temp;
        std::swap(lhs.size_, rhs.size_);
    }

} // my::
