#include "webapi.hpp"
#include "id_manager.hpp"
#include <unordered_map>
#include <boost/range/algorithm_ext.hpp>
#include <boost/process.hpp>
#include <utility/get_exe_path.hpp>

namespace utility
{
    BasicID::BasicID(const Type type_, const int value_) : impl_(std::make_shared<Impl>(type_, value_))
    {
        IDManager::get_instance().reg_id(impl_);
    }

    BasicID::BasicID(const Type type_, const int value_, const std::string &uid_) : impl_(std::make_shared<Impl>(type_, value_, uid_))
    {
        IDManager::get_instance().reg_id(impl_);
    }

    BasicID::BasicID(const BasicID &other) : impl_(other.impl_)
    {}

    BasicID::~BasicID() = default;

    void BasicID::set_value(int value_)
    {
        impl_->value = value_;
    }
    
    int BasicID::value() const
    {
        return impl_->value;
    }

    std::string BasicID::uid() const
    {
        if (!impl_->uid)
        {
            IDManager::get_instance().set_ids();
        }
        return impl_->uid.value();
    }

    void BasicID::set_uid(const std::string& uid_)
    {
        impl_->uid = uid_;
    }


    IDManager::IDManager() = default;
    IDManager::~IDManager() = default;

    IDManager &IDManager::get_instance()
    {
        static IDManager im_;
        return im_;
    }

    void IDManager::reg_id(const std::shared_ptr<BasicID::Impl> &sp)
    {
        ids.push_back(sp);
    }

    void IDManager::set_ids()
    {
        namespace bp = boost::process;
        std::unordered_map<std::string, std::vector<std::shared_ptr<BasicID::Impl>>> types;
        for (const auto &wp : ids)
        {
            if (wp.expired())
            {
                continue;
            }
            std::shared_ptr<BasicID::Impl> sp = wp.lock();
            if (!sp->uid)
            {
                types[BasicID::type_str(sp->type)].push_back(sp);
            }
        }
        std::vector<json11::Json> requests;
        for (auto &kv : types)
        {
            auto type = kv.first;
            auto num = static_cast<int>(kv.second.size());
            requests.emplace_back(json11::Json({
                { "type", type },
                { "num", num }
                }));
        }
        json11::Json::object input({
            { "requests", requests }
            });
        bp::opstream in;
        bp::ipstream out;
        bp::child c(utility::get_exe_path() + "distribute_uid", bp::std_out > out, bp::std_in < in);
        in << json11::Json(input).dump() << std::endl;
        in.flush();
        in.pipe().close();
        std::string r { std::istreambuf_iterator<char>(out), std::istreambuf_iterator<char>() };
        c.wait();

        if (r.empty())
        {
            throw std::runtime_error("Error: cannot get ID(s).");
        }

        std::string err;
        const auto res = json11::Json::parse(r, err);
        for (auto& rec : res["results"].array_items())
        {
            auto target = types[rec["type"].string_value()];
            for (auto i = 0; i < target.size(); ++i)
            {
                std::string uid = rec["ids"].array_items()[i].string_value();
                target[i]->uid = uid;
            }
        }
    }

    template<class Iter>
    std::ostream& to_ostream(std::ostream& stream, Iter begin, Iter end)
    {
        stream << "[";
        if ( begin != end )
        {
            stream << begin->uid();

            for ( ; ++begin != end; )
            {
                stream << ", " << begin->uid();
            }
        }
        stream << "]";

        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const std::vector<TopologyID>& src)
    {
        return to_ostream(stream, std::begin(src), std::end(src));
    }

    std::ostream& operator<<(std::ostream& stream, const std::vector<FeatureID>& src)
    {
        return to_ostream(stream, std::begin(src), std::end(src));
    }
}
