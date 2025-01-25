#pragma once
#include <string>
#include "json/json11util.hpp"

namespace utility
{
    class WebAPI
    {
        std::string host;
        int port;
        std::string target;
    public:
        WebAPI(std::string host_, int port_, std::string target_);
        std::string get(std::string body);
        json11::Json call(json11::Json &input);
        json11::Json call(json11::Json &&input);
    };
}