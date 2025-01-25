/**
\file composed_of.hpp
\brief Feature 列の Type を確認する utility
*/

#pragma once

#include <algorithm>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/cxx11/none_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range/adaptors.hpp>

namespace utility
{
    namespace algorithm
    {
        template <typename Types, typename Features>
        bool is_composed_of(const Types& types, const Features& targets)
        {
            return boost::equal(targets
                | boost::adaptors::transformed([](const typename Features::value_type t)
            {
                return t->type();
            }),
                types);
        }
    }
}
