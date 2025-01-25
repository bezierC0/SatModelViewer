#pragma once

#include <string>

namespace utility
{
    namespace uri
    {
        // 2020-09-18 用途限定
        struct parsed_uri
        {
            std::string host; 
            int         port;
            std::string path;
        };
        parsed_uri parse(const std::string& uri_str);
    }
}
