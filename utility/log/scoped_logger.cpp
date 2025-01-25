/**
\file scoped_logger.cpp
\brief オブジェクトの生成時と破棄時にログを書き込むクラス
*/

#include "scoped_logger.hpp"

#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>

namespace utility
{
    ScopedLogger::ScopedLogger(boost::log::trivial::severity_level l, const std::string& c, const std::string& d)
        : lvl(l), str_when_destruct(d)
    {
        // BOOST_LOG_TRIVIAL と同等
        BOOST_LOG_SEV(boost::log::trivial::logger::get(), l) << c;
    }

    ScopedLogger::~ScopedLogger()
    {
        BOOST_LOG_SEV(boost::log::trivial::logger::get(), lvl) << str_when_destruct;
    }
}
