#pragma once

#include <vector>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/adaptors.hpp>
#include "utility/null.hpp"

namespace utility
{
    /**
     *@brief feature_filter<A*>(src)   srcのうちAにキャストできる要素の集合を、vector<A*>として返す。
     *@brief feature_filter<A*,B*>(src) srcのうちAにキャストできる要素の集合を、vector<B*>として返す。
     */
    template<class FilterTargetType, class DestStoreType = FilterTargetType, class Container>
    auto feature_filter(const Container& src)
    {
        std::vector<DestStoreType> dest;
        for (auto&& s : src)
        {
            if (auto casted = dynamic_cast<FilterTargetType>(s))
            {
                dest.push_back(casted);
            }
        }
        return dest;
    }

    /**
    \brief 指定した型のPrimitiveを抽出する
    \brief primitive_typeを参照しないfilter
    \param range Primitiveの集合
    \return 抽出結果の集合
    */
    template<typename T, typename Range>
    std::vector<const T*> _filter(const Range& range)
    {
        using namespace boost::adaptors;
        std::vector<const T*> results;
        boost::push_back(results,
            range
            | transformed([](const auto* p) { return dynamic_cast<const T*>(p); })
            | filtered(utility::is_not_null<const T*>));
        return std::move(results);
    }
}

