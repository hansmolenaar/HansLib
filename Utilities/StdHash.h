#pragma once

#include "Rational.h"
#include <array>
#include <boost/functional/hash.hpp>

template<typename T, size_t N>
struct ArrayHasher {
    std::size_t operator()(const std::array<T, N>& a) const {
        std::size_t h = 0;

        for (auto e : a) 
	{
           boost::hash_combine(h, std::hash<T>{}(e) );
        }
        return h;
    }   
};
