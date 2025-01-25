/**
\file scoped_logger.hpp
\brief オブジェクトの生成時と破棄時にログを書き込むクラス
*/

#pragma once

#include <string>
#include <boost/log/trivial.hpp>
#include <boost/scope_exit.hpp>

namespace utility
{
    class ScopedLogger
    {
    public:
        ScopedLogger(boost::log::trivial::severity_level lvl, const std::string& str_when_construct, const std::string& str_when_destruct);

        ScopedLogger(const std::string& str_when_construct, const std::string& str_when_destruct)
            : ScopedLogger(boost::log::trivial::info, str_when_construct, str_when_destruct){}

        ~ScopedLogger();
    private:
        boost::log::trivial::severity_level lvl;
        const std::string str_when_destruct;
    };

#if (defined(__linux__) && defined(NDEBUG)) || (defined(_WIN32) && !defined(_DEBUG))
#define TRACE_ENTER_EXIT
#else
#define TRACE_ENTER_EXIT utility::ScopedLogger _trace_enter_exit_loggr(boost::log::trivial::trace, std::string("enter ").append(__FUNCTION__), std::string("exit ").append(__FUNCTION__));
#endif
}
