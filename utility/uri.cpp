#include "uri.hpp"
#include <cstring>

namespace
{
    // 文字列を特定文字で前後分割した結果
    struct splitted_t
    {
        struct
        {
            const char* begin;
            const char* end;
        } first{}, second{};

        operator bool() const
        {
            return first.begin && first.end && second.begin && second.end;
        }
    };

    // [begin, end) 間の文字列を、splitterで分割する。結果にsplitter部は含まない。
    splitted_t split(const char* src_begin, const char* src_end, char splitter)
    {
        for ( auto p = src_begin; p != src_end; ++p )
        {
            if ( *p == splitter )
            {
                return {{src_begin, p}, {p + 1, src_end}};
            }
        }
        return {};
    }
    splitted_t split(const char* src, char splitter)
    {
        return split(src, src + strlen(src), splitter);
    }
} // anonymous 

namespace utility
{
    namespace uri
    {
        constexpr int DEFAULT_PORT = 80;

        // 参考: https://en.wikipedia.org/wiki/Uniform_Resource_Identifier
        // 参考: https://triple-underscore.github.io/rfc-others/RFC3986-ja.html
        parsed_uri parse(const std::string& uri_str)
        {
            parsed_uri result {"", DEFAULT_PORT, ""};

            if ( auto begin = strstr(uri_str.c_str(), "://") )
            {
                begin += 3; // length of "://"

                auto end = strrchr(begin, '\0');
                if ( auto slash = split(begin, end, '/') )
                {
                    result.host.assign(begin, slash.first.end);

                    if ( auto questionmark = split(slash.second.begin, end, '?') )
                    {
                        result.path.assign(slash.first.end /* slash include */, questionmark.first.end);
                    }
                    else // query部 なし
                    {
                        result.path.assign(slash.first.end /* slash include */, end);
                    }
                }
                else // path部 なし(==host部 [+port] のみ)。※pathなし==queryなし
                {
                    result.host.assign(begin, end);
                }

                // host部にport指定あれば分割
                auto phost = result.host.c_str();
                if ( auto port = split(phost, ':') )
                {
                    result.port = atoi(port.second.begin);
                    result.host.assign(port.first.begin, port.first.end);
                }
            }

            return result;
        }
    }
}
