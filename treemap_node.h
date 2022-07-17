// C++ Aufgabe treemap von Hartmut Schirmacher, BHT Berlin

#pragma once

// class definitions
#include "treemap_class.h"
#include <memory>



namespace my {

    // this is an inner class of class my::tree_map
    template<typename K, typename T>
    class treemap<K,T>::node 
        : public std::enable_shared_from_this<node> 
    {

    public:
        std::weak_ptr<node> parent_;
        std::shared_ptr<node> left_, right_;
        value_type data_;

        treemap<K, T>::node(value_type vt, std::shared_ptr<node> parent = nullptr)
            : parent_(parent), left_(nullptr), right_(nullptr), data_(vt)
            {
            }
    };



} // my::