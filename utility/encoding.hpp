#if defined(_WIN64)

#pragma once

#include <boost/optional.hpp>

namespace utility
{
    namespace encoding
    {
        boost::optional<std::string> utf8_to_current_code_page(const std::string& utf8_text);
    }
}

#endif