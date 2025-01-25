#include "utility/id_counter.hpp"

namespace utility
{
    IDCounter::IDCounter() : id(0) {}
    IDCounter::IDCounter(int start_id) : id(start_id) {}

    /**
    \brief ID を取得する
    \return ID
    \note ID を取得すると副作用として内部で保持しているIDを1つ進める
    */
    int IDCounter::get() { return id++; }
}
