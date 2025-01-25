#include "bitset.hpp"

namespace utility
{
    /**
    \brief bit全探索を行う
    \details 全てのビットの組合せ分、funcを実行する
    \param n bitsetのサイズ (nビット)
    \param func 実行される関数 (void func(const boost::dynamic_bitset&) 形式)
    \remark 全ビットOFF(空集合)は処理しない
    \remark 降順(全ビットON → 全ビットOFF)に処理する
    */
    void for_each_power_set_bits(std::size_t n, std::function<void(const boost::dynamic_bitset<>&)> func)
    {
        if (n == 0)
        {
            return;
        }
        // bit全探索
        for (auto b = boost::dynamic_bitset<>(n).flip(); !b.none(); b = decrement(b))
        {
            func(b);
        }
    }
}

