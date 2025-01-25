#pragma once
#include <boost/variant.hpp>
template<typename Left, typename Right>
class Either
{
public:
    Either(const Left& l):impl(l) {}
    Either(const Right& r):impl(r) {}
    const Left& left() const { return boost::get<Left>(impl); }
    Left& left() { return boost::get<Left>(impl); }
    const Right& right() const { return boost::get<Right>(impl); }
    Right& right() { return boost::get<Right>(impl); }
    template<typename F>
    Either<Left, Right> fmap(F f)
    {
        if (impl.which() == 1)
            return f(right());
        else
            return left();
    }
    explicit operator bool() const { return impl.which() == 1; }
private:
    boost::variant<Left, Right> impl;
};
