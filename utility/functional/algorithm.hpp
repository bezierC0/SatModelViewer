/**
\file algorithm.hpp
\brief <algorithm> に含まれる関数の関数オブジェクト版
*/

#pragma once

#include <algorithm>

namespace utility
{
    template <class T>
    struct min
    {
        const T& operator() (const T& lhs, const T& rhs) const { return std::min<T>(lhs, rhs); }
        typedef T first_argument_type;
        typedef T second_argument_type;
        typedef bool result_type;
    };

    template <class T>
    struct max
    {
        const T& operator() (const T& lhs, const T& rhs) const { return std::max<T>(lhs, rhs); }
        typedef T first_argument_type;
        typedef T second_argument_type;
        typedef bool result_type;
    };
}
