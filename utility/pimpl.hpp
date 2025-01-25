#pragma once

#define PIMPL_NONCOPYABLE(member_name) private: \
        struct Impl; \
        std::unique_ptr<Impl> member_name;

#define PIMPL_COPYABLE(member_name) private: \
        struct Impl; \
        std::shared_ptr<Impl> member_name;

#define PIMPL_COPYABLE_WITH_TEMPLATE(member_name, T) private: \
        template<typename T> struct Impl; \
        std::shared_ptr<Impl<T>> member_name;
