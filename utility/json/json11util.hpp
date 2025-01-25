#ifndef JSON11UTIL_HPP_INCLUDED_HELPERFUNCTIONS_FOR_JSON11_20180314_
#define JSON11UTIL_HPP_INCLUDED_HELPERFUNCTIONS_FOR_JSON11_20180314_

// 2018-03-29 dig()のパス区切り指定できるよう改善
// 2018-03-30 deep_compare()追加
// 2018-06-05 jsonパスの保持(breadcrumbs_t)をクラスにした

#include "json11.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <initializer_list>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include "utility/picojson/picojson.h"


namespace json11util
{
    inline json11::Json readJson(const std::string& path, std::string& errstr)
    {
        std::ifstream ifstream(path);
        if ( ! ifstream )
        {
            errstr = std::string {"File open error: "}.append(path);
            return {};
        }

        std::stringstream sstream;
        sstream << ifstream.rdbuf();
        std::string jsonstr {sstream.str()};

        return json11::Json::parse(jsonstr, errstr);
    }

    inline std::string /* error */ readJson(json11::Json& dest, const std::string& path)
    {
        std::string errstr;
        dest = readJson(path, errstr);
        return errstr;
    }

    inline json11::Json readJson(const std::string& path)
    {
        json11::Json result;
        auto errstr = readJson(result, path);

        return errstr.empty()? result: json11::Json{};
    }

    inline bool contain(const json11::Json& parent, const std::string& key)
    {
        return parent.object_items().count(key) != 0u;
    }

    template<typename StrType>
    inline bool contain(const json11::Json& parent, std::initializer_list<StrType> keys)
    {
        auto& obj = parent.object_items();
        return std::all_of(std::begin(keys), std::end(keys), [&](auto& key){return obj.count(key) != 0u;});
    }

#if defined picojson_h /* picojson.h included */
    inline json11::Json from_picojson(const picojson::value& pj)
    {
        std::string err;
        auto result = json11::Json::parse(pj.serialize(), err);
        if ( err.empty() )
        {
            return result;
        }
        throw std::runtime_error(err); // picojson::valueとして構築できているから、変換でコケることはないはず
    }
#endif

    inline const json11::Json& value(const json11::Json& parent, const std::string& key)
    {
        try
        {
            return parent.is_array()?
                parent[std::stoull(key)]: /* 32bit版のコンパイルならここでWarning */
                parent[key];
        }
        catch (...) // std::invalid_argument, std::out_of_range 数値への変換失敗
        {
            static json11::Json niljson;
            return niljson;
        }
    }

    inline json11::Json dig(json11::Json parent, const std::string& path, std::string* error = nullptr, char path_delimiter = '.')
    {
        std::istringstream pathstr_stream {path};
        std::string token;

        for ( ; ! parent.is_null() && std::getline(pathstr_stream, token, path_delimiter); parent = value(parent, token) )
            ;

        if ( parent.is_null() && error ) *error = token;
        return parent;
    }

    // template引数にする必要性がなかった
    template<char PathDelimiter = '.'>
    [[deprecated]] inline json11::Json dig(json11::Json parent, const std::string& path, std::string* error = nullptr)
    {
        return dig(parent, path, error, PathDelimiter);
    }

    inline std::string type2name(json11::Json::Type json11type)
    {
        //enum Type {
        //    NUL, NUMBER, BOOL, STRING, ARRAY, OBJECT
        //};
        static_assert(json11::Json::OBJECT == 5, "json11::Json::Type may have changed");

        static const char* names[] = {
            "nul", "number", "bool", "string", "array", "object"
        };
        return names[(int)json11type];
    };

    class breadcrumbs_t
    {
    private:
        const char delimiter_;
        std::deque<std::string> breadcrumbs;

    public:
        breadcrumbs_t(char path_delimiter = '.', const std::string& path = {})
        : delimiter_(path_delimiter)
        {
            if ( ! path.empty() )
            {
                for ( auto&& begin = path.c_str(), iter = begin, end = begin + path.length(); ; )
                {
                    if ( *iter == delimiter_ || *(++iter) == delimiter_ )
                    {
                        breadcrumbs.emplace_back(begin, iter);
                        begin = ++iter;
                    }
                    if ( iter == end )
                    {
                        breadcrumbs.emplace_back(begin, iter);
                        break;
                    }
                }
            }
        }

        // boost::format::operator%()
        friend std::ostream& operator<<(std::ostream& os, const breadcrumbs_t& bc)
        {
            if ( bc.breadcrumbs.empty() )
            {
                os << "root";
            }
            else
            {
                std::string joined;
                for ( auto&& token: bc.breadcrumbs )
                {
                    joined.append(token).append(1u, bc.delimiter_);
                }
                os << joined;
            }
            return os;
        }

        template<class StrType>
        void push(StrType&& token)
        {
            breadcrumbs.push_back(token);
        }

        void pop()
        {
            breadcrumbs.pop_back();
        }
    };

    inline std::string deep_compare(const json11::Json& tst, const json11::Json& ans, breadcrumbs_t& breadcrumbs, char path_delimiter = '.')
    {
        auto tst_type = tst.type(), ans_type = ans.type();
        if ( tst_type != ans_type )
        {
            return (boost::format{"Type unmatch at %1%. %2% should be %3%"} % breadcrumbs % type2name(tst_type) % type2name(ans_type)).str();
        }

        switch (tst_type)
        {
        case json11::Json::OBJECT:
        {
            auto& tst_obj = tst.object_items();
            auto& ans_obj = ans.object_items();
            auto tst_sz = tst_obj.size(), ans_sz = ans_obj.size();
            if ( tst_sz != ans_sz )
            {
                return (boost::format{"Object item count doesn't match at %1%. %2% should be %3%"} % breadcrumbs % tst_sz % ans_sz).str();
            }

            // 順序はmapが保証している
            for ( auto tst_iter = std::begin(tst_obj), ans_iter = std::begin(ans_obj), tst_end = std::end(tst_obj); tst_iter != tst_end; ++tst_iter, ++ans_iter )
            {
                auto& tst_name = tst_iter->first;
                auto& ans_name = ans_iter->first;
                if ( tst_name != ans_name )
                {
                    return (boost::format{"Key name doesn't match at %1%. %2% should be %3%"} % breadcrumbs % tst_name % ans_name).str();
                }

                breadcrumbs.push(tst_name);
                auto err_of_thisitem = deep_compare(tst_iter->second, ans_iter->second, breadcrumbs);
                if ( ! err_of_thisitem.empty() ) return err_of_thisitem;
                breadcrumbs.pop();
            }
            break;
        }
        case json11::Json::ARRAY:
        {
            auto& tst_arr = tst.array_items();
            auto& ans_arr = ans.array_items();
            auto tst_sz = tst_arr.size(), ans_sz = ans_arr.size();
            if ( tst_sz != ans_sz )
            {
                return (boost::format{"Array item count doesn't match at %1%. %2% should be %3%"} % breadcrumbs % tst_sz % ans_sz).str();
            }

            for ( auto n = 0u; n < tst_sz; ++n )
            {
                breadcrumbs.push( std::to_string(n) );
                auto err_of_thisitem = deep_compare(tst_arr[n], ans_arr[n], breadcrumbs);
                if ( ! err_of_thisitem.empty() ) return err_of_thisitem;
                breadcrumbs.pop();
            }
            break;
        }
        default: // string, number, boolean and 'nil' <- nil行けるかどうか未確認。たぶん平気
        {
            if ( tst != ans )
            {
                return (boost::format{"Value doesn't match at %1%. %2% should be %3%"} % breadcrumbs % tst.dump() % ans.dump()).str();
            }
            break;
        }
        };

        return {};
    }

    template<char PathDelimiter = '.'>
    [[deprecated]] inline std::string deep_compare(const json11::Json& tst, const json11::Json& ans, breadcrumbs_t& breadcrumbs)
    {
        return deep_compare(tst, ans, breadcrumbs, PathDelimiter);
    }

    template<char PathDelimiter = '.'>
    [[deprecated]] inline std::string deep_compare(const json11::Json& test_json, const json11::Json& answer_json)
    {
        breadcrumbs_t breadcrumbs;
        return deep_compare(test_json, answer_json, breadcrumbs, PathDelimiter);
    }

    inline std::string deep_compare(const json11::Json& test_json, const json11::Json& answer_json, char path_delimiter = '.')
    {
        breadcrumbs_t breadcrumbs;
        return deep_compare(test_json, answer_json, breadcrumbs, path_delimiter);
    }

    inline std::string validate(const json11::Json& answer,       // 正解json(rootから全体) 
                                const json11::Json& frresult,     // 検証するfr_result(picojsonから変換した全体)
                                breadcrumbs_t& breadcrumbs)       // 誤り検出時に使うjsonパスの足あと
    {
        auto ans_type = answer.type(), tst_type = frresult.type();        if ( ans_type != tst_type )
        {
            auto dump_ans = answer.dump();
            auto dump_tst = frresult.dump();

            return (boost::format{"Type unmatch at %1%. %2% should be %3%"} % breadcrumbs % type2name(tst_type) % type2name(ans_type)).str();
        }

        switch (ans_type)
        {
        case json11::Json::OBJECT: // answer側に存在する要素だけ評価する。ex."answerside.keyname" not found in testside -> validate failed
        {
            auto& tst_obj = frresult.object_items();

            // 順序はmapが保証している
            for ( auto&& ans_item: answer.object_items() ) // 要素数: answerside ≦ testside
            {
                auto ans_keyname = ans_item.first;
                auto tst_iter = tst_obj.find(ans_keyname);
                if ( tst_iter == std::end(tst_obj) )
                {
                    return (boost::format{"Key %1% not found at test-side. path: %2%"} % ans_keyname % breadcrumbs).str();
                }

                breadcrumbs.push(ans_keyname);
                auto err_of_thisitem = validate(ans_item.second, tst_iter->second, breadcrumbs);
                if ( ! err_of_thisitem.empty() ) return err_of_thisitem;
                breadcrumbs.pop();
            }

            break;
        }
        case json11::Json::ARRAY: // answer側の要素だけ評価するが、objectと違い相手方を特定するKeyが無い..色々試みたが軽く挫折。arrayは要素を順になめることにする。
        {
            auto& ans_arr = answer.array_items();
            auto& tst_arr = frresult.array_items();

            auto ans_sz = ans_arr.size(), tst_sz = tst_arr.size(); // 軽く挫折したので、要素数一致を条件にする。
            if ( ans_sz != tst_sz )
            {
                return (boost::format{"Array item count doesn't match at %1%. %2% should be %3%"} % breadcrumbs % tst_sz % ans_sz).str();
            }

            for ( auto n = 0u; n < ans_sz; ++n ) // indexがほしい
            {
                breadcrumbs.push( std::to_string(n) );
                auto err = validate(ans_arr[n], tst_arr[n], breadcrumbs);
                if ( ! err.empty() ) return err;
                breadcrumbs.pop();
            }
            break;
        }
        default: // string, number, boolean and 'nil' <- nil行けるかどうか未確認。たぶん平気
        {
            if ( answer != frresult )
            {
                return (boost::format{"Value doesn't match at %1%. %2% should be %3%"} % breadcrumbs % frresult.dump() % answer.dump()).str();
            }
            break;
        }
        };

        return {};
    }

    // answerが検証箇所だけ抜粋済み
    inline std::string validate(const json11::Json& answer,       // 正解json(rootから全体) 
                                const json11::Json& frresult,     // 検証するfr_result(picojsonから変換した全体)
                                char path_delimiter = '.')        // 誤り検出時に使うjsonパスの区切り文字
    {
        breadcrumbs_t bc{path_delimiter};
        return validate(answer, frresult, bc);
    }

    // 検証個所をpath_to_testで指定
    inline std::string validate(const json11::Json& answer,       // 正解json(rootから全体) 
                                const json11::Json& frresult,     // 検証するfr_result(picojsonから変換した全体)
                                std::string&&  path_to_test,      // 検証地点までのjsonパス
                                char path_delimiter = '.')        // path_to_testおよび誤り検出時に使うjsonパスの区切り文字
    {
        breadcrumbs_t bc{path_delimiter, path_to_test};
        return validate(dig(answer, path_to_test, nullptr, path_delimiter), 
                        dig(frresult, path_to_test, nullptr, path_delimiter), 
                        bc);
    }

    inline auto intarr(const json11::Json::array& arr)
    {
        return arr | boost::adaptors::transformed([](auto&& v){return v.int_value();});
    }

    template<template<class, class> class ContainerType = std::vector>
    auto intarr_conv(const json11::Json::array& arr)
    {
        auto converted = intarr(arr);
        return ContainerType<int, std::allocator<int> >{std::begin(converted), std::end(converted)};
    }

    inline auto doublearr(const json11::Json::array& arr)
    {
        return arr | boost::adaptors::transformed([](auto&& v){return v.number_value();});
    }

    template<template<class, class> class ContainerType = std::vector>
    auto doublearr_conv(const json11::Json::array& arr)
    {
        auto converted = doublearr(arr);
        return ContainerType<double, std::allocator<double> >{std::begin(converted), std::end(converted)};
    }

    inline auto strarr(const json11::Json::array& arr)
    {
        return arr | boost::adaptors::transformed([](auto&& v) { return v.string_value(); });
    }

    template<template<class, class> class ContainerType = std::vector>
    auto strarr_conv(const json11::Json::array& arr)
    {
        auto converted = strarr(arr);
        return ContainerType<std::string, std::allocator<std::string> >{std::begin(converted), std::end(converted)};
    }

} // namespace json11util

#endif // JSON11UTIL_HPP_INCLUDED_HELPERFUNCTIONS_FOR_JSON11_20180314_
