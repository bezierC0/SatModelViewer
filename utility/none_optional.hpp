/**
\file none_optional.hpp
\brief none optional 判定
*/

#pragma once

namespace utility
{
    template<class T>
    bool is_none(boost::optional<T>& opt)
    {
        return bool(opt) == false;
    }

    template<class T>
    bool is_not_none(boost::optional<T>& opt)
    {
        return bool(opt) == true;
    }
}
