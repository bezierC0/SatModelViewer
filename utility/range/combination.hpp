#pragma once

#include <vector>
#include <boost/algorithm/cxx11/any_of.hpp>

namespace utility
{
    /**
    \brief 集合から2要素を取り出す組み合わせを生成する
    \param range 集合
    \return 組み合わせの集合
    */
    template<typename Range>
    auto combination(const Range& range)
    {
        using value_type = typename Range::value_type;
        std::vector<std::pair<value_type, value_type>> ret;
        const auto ed = std::end(range);
        for (auto i = std::begin(range); i != ed; ++i)
        {
            for (auto j = i + 1; j != ed; ++j)
            {
                ret.emplace_back(*i, *j);
            }
        }
        return ret;
    }

    /**
    \brief 2つの集合の直積で、predを満たすペアが存在するか
    \param dst 集合1
    \param src 集合2
    \param pred 条件関数
    \return true: 存在する, false: 存在しない
    */
    template<typename Range1, typename Range2, typename Predicate>
    bool exists_of(Range1& dst, Range2& src, Predicate pred)
    {
        return boost::algorithm::any_of(dst, [&src, &pred](auto d)
            {
                return boost::algorithm::any_of(src, [&d, &pred](auto s)
                    {
                        return pred(d, s);
                    });
            });
    }
}

