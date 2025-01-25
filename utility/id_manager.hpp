#pragma once
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <ostream>

namespace utility
{
    class BasicID
    {
    protected:
        enum Type
        {
            Topology,
            Feature
        };
    private:
        struct Impl
        {
            Impl(const Type type_, const int value_)
                : type(type_), value(value_), uid(boost::none)
            {}
            Impl(const Type type_, const int value_, const std::string uid_)
                : type(type_),  value(value_), uid(uid_)
            {}
            ~Impl() = default;
            Type type;
            int value;
            boost::optional<std::string> uid;
        };
        std::shared_ptr<Impl> impl_;
        friend class IDManager;
        static std::string type_str(const Type type_)
        {
            switch (type_)
            {
            case Topology:
                return "topology";
            case Feature:
                return "feature";
            default:
                return "";
            }
        }
        Impl *get() const
        {
            return impl_.get();
        }

    public:
        BasicID(Type type_, int value_);
        BasicID(Type type_, int value_, const std::string &uid_);
        BasicID(const BasicID &other);
        ~BasicID();

        int value() const;
        void set_value(int value_);

        void set_uid(const std::string &);
        std::string uid() const;

        operator int() const
        {
            return value();
        }
    };

    class TopologyID : public utility::BasicID
    {
    public:
        TopologyID(const int value_ = 0) : BasicID(Topology, value_) {}
        TopologyID(const int value_, const std::string &uid_) : BasicID(Topology, value_, uid_) {}
    };


    class FeatureID : public BasicID
    {
    public:
        FeatureID(const int value_ = 0) : BasicID(Feature, value_) {}
        FeatureID(const int value_, const std::string &uid_) : BasicID(Feature, value_, uid_) {}
    };

    class IDManager
    {
        IDManager();
        ~IDManager();
        std::vector<std::weak_ptr<BasicID::Impl>> ids;
        friend class BasicID;
        static IDManager& get_instance();
        void reg_id(const std::shared_ptr<BasicID::Impl>&);
        void set_ids();
    public:
        IDManager(const IDManager&) = delete;
        IDManager& operator=(const IDManager&) = delete;
        IDManager(IDManager&&) = delete;
        IDManager& operator=(IDManager&&) = delete;
    };

    class IDPool
    {
        using TopologyID = utility::TopologyID;
        std::vector<TopologyID> pool;
        int counter;
    public:
        IDPool(size_t sz) : counter(0), pool(sz)
        {}
        ~IDPool() = default;

        TopologyID& get_one()
        {
            return pool[counter++];
        }
    };

    // TopologyID, FeatueID実体のコンテナなので、BasicIDに集約できない
    std::ostream& operator<<(std::ostream& stream, const std::vector<TopologyID>& src);
    std::ostream& operator<<(std::ostream& stream, const std::vector<FeatureID>& src);
}
