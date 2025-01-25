/**
\file exception.hpp
\brief Error Handling 関連ユーティリティ
*/

#pragma once

#include <string>
#include <stdexcept>

#define THROW_LOGIC_ERROR(message)    \
    throw std::logic_error(std::string("logic_error: ").append(message).append(" (in function ").append(__func__).append(")"))
