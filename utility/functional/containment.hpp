/**
\file containment.hpp
\brief
*/

#pragma once

#include <algorithm>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/cxx11/none_of.hpp>
#include <boost/utility/enable_if.hpp>

namespace utility
{
    namespace detail
    {
        struct has_find_impl
        {
            template <typename T, typename V>
            static auto check(T*, V* v) -> decltype(std::declval<T>().find(std::declval<V>()), std::true_type());

            template <typename T, typename V>
            static auto check(...)->std::false_type;
        };

        template <typename T, typename V>
        struct has_find : decltype(has_find_impl::check<T, V>(nullptr, nullptr)) {};

        template <typename Range, typename Value, typename boost::enable_if<has_find<Range, Value>>::type* = nullptr>
        inline bool none_of_equal_impl(const Range& range, const Value& val) { return range.find(val) == range.end(); }

        template <typename Range, typename Value, typename boost::disable_if<has_find<Range, Value>>::type* = nullptr>
        inline bool none_of_equal_impl(const Range& range, const Value& val) { return boost::algorithm::none_of_equal(range, val); }

        template <typename Range, typename Value, typename boost::enable_if<has_find<Range, Value>>::type* = nullptr>
        inline bool any_of_equal_impl(const Range& range, const Value& val) { return range.find(val) != range.end(); }

        template <typename Range, typename Value, typename boost::disable_if<has_find<Range, Value>>::type* = nullptr>
        inline bool any_of_equal_impl(const Range& range, const Value& val) { return boost::algorithm::any_of_equal(range, val); }

        template <typename Range>
        struct is_excluded_from
        {
            using value_type = decltype(*std::begin(std::declval<Range>()));

            const Range& range;

            is_excluded_from(const Range& r) :range(r) {}

            bool operator() (const value_type val) const
            {
                return detail::none_of_equal_impl<Range, value_type>(range, val);
            }
        };

        template <typename Range1>
        struct are_excluded_from
        {
            are_excluded_from(const Range1& r) :range(r) {}

            template <typename Range2>
            bool operator() (const Range2& val) const
            {
                return boost::algorithm::all_of(val, is_excluded_from<Range1>(range));
            }

            const Range1& range;
        };

        template <typename Range>
        struct is_included_in
        {
            using value_type = decltype(*std::begin(std::declval<Range>()));

            is_included_in(const Range& r) :range(r) {}

            bool operator() (const value_type val) const
            {
                return detail::any_of_equal_impl<Range, value_type>(range, val);
            }

            const Range& range;
        };

        template <typename Range1>
        struct are_included_in
        {
            are_included_in(const Range1& r) :range(r) {}

            template <typename Range2>
            bool operator() (const Range2& val) const { return boost::algorithm::all_of(val, is_included_in<Range1>(range)); }

            const Range1& range;
        };

        template <typename Range1>
        struct are_intersected_with
        {
            are_intersected_with(const Range1& r) :range(r) {}

            template <typename Range2>
            bool operator() (const Range2& val) const { return boost::algorithm::any_of(val, is_included_in<Range1>(range)); }

            const Range1& range;
        };

    }

    // std::function を返すと type erasure によって最適化が妨げられるので、明示的な関数オブジェクトを返す。

    /**
    \brief 引数が range に含まれなければ true を返す関数を返す。
    */
    template <typename Range>
    detail::is_excluded_from<Range> is_excluded_from(const Range& range)
    {
        return detail::is_excluded_from<Range>(range);
    }

    /**
    \brief 引数の要素がすべて range に含まれなければ true を返す関数を返す。
    */
    template <typename Range>
    detail::are_excluded_from<Range> are_excluded_from(const Range& range)
    {
        return detail::are_excluded_from<Range>(range);
    }

    /**
    \brief 引数が range に含まれたら true を返す関数を返す。
    */
    template <typename Range>
    detail::is_included_in<Range> is_included_in(const Range& range)
    {
        return detail::is_included_in<Range>(range);
    }

    /**
    \brief 引数の要素がすべて range に含まれたら true を返す関数を返す。
    */
    template <typename Range>
    detail::are_included_in<Range> are_included_in(const Range& range)
    {
        return detail::are_included_in<Range>(range);
    }

    /**
    \brief 引数の要素のいずれかが range に含まれたら true を返す関数を返す。
    */
    template <typename Range>
    detail::are_intersected_with<Range> are_intersected_with(const Range& range)
    {
        return detail::are_intersected_with<Range>(range);
    }

    /**
    \brief val が range に含まれなかったら true を返す。
    */
    template <typename Range>
    bool is_excluded_from(const Range& range, typename Range::value_type val)
    {
        return is_excluded_from<Range>(range)(val);
    }

    /**
    \brief range2 のすべての要素が range1 に含まれなかったら true を返す。
    */
    template <typename Range1, typename Range2>
    bool are_excluded_from(const Range1& range1, const Range2& range2)
    {
        return are_excluded_from<Range1>(range1)(range2);
    }

    /**
    \brief val が range に含まれたら true を返す。
    */
    template <typename Range>
    bool is_included_in(const Range& range, typename Range::value_type val)
    {
        return is_included_in<Range>(range)(val);
    }

    /**
    \brief range2 のすべての要素が range1 に含まれたら true を返す。
    */
    template <typename Range1, typename Range2>
    bool are_included_in(const Range1& range1, const Range2& range2)
    {
        return are_included_in<Range1>(range1)(range2);
    }

    /**
    \brief range2 の要素のいずれかが range1 に含まれたら true を返す。
    */
    template <typename Range1, typename Range2>
    bool are_intersected_with(const Range1& range1, const Range2& range2)
    {
        return are_intersected_with<Range1>(range1)(range2);
    }

    /**
    \brief 2つの集合が同値か
    \details 重複のない要素が全て同じか
    \details 則ち(range1 ⊆ range2) && (range1 ⊇ range2)
    */
    template <typename Range1, typename Range2>
    bool are_same_with(const Range1& range1, const Range2& range2)
    {
        return are_included_in(range1, range2) && are_included_in(range2, range1);
    }
}
