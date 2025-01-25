/**
 \file null.hpp
 \brief null 判定
 */

#pragma once

namespace utility
{
    template<class T>
    bool is_null(T p)
    {
        return p == nullptr;
    }

    template<class T>
    bool is_not_null(T p)
    {
        return p != nullptr;
    }
}
