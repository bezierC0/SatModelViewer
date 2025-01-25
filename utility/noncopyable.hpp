#pragma once

#define DISALLOW_COPY_MOVE_ASSIGN(TypeName) public: \
        TypeName() = delete; \
        TypeName(const TypeName&)= delete; \
        TypeName(TypeName&&) = delete; \
        void operator=(const TypeName&) = delete; \
        void operator=(TypeName&&) = delete;
