#include <functional>
#include <boost/dynamic_bitset.hpp>

namespace utility
{
    /**
    \brief bitsetをデクリメントする
    */
    template<typename Block, typename Alloc>
    boost::dynamic_bitset<Block, Alloc> decrement(const boost::dynamic_bitset<Block, Alloc>& bits)
    {
        auto ret = bits;
        const auto n = ret.size();
        for (auto i = 0; i < n; ++i)
        {
            if (ret.test(i))
            {
                ret.reset(i);
                break;
            }
            else
            {
                ret.set(i);
            }
        }
        return ret;
    }

    void for_each_power_set_bits(std::size_t n, std::function<void(const boost::dynamic_bitset<>&)> func);
}

