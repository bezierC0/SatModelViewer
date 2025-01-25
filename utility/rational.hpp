/**
\file rational.hpp
\brief 有理数
*/

#pragma once
#include <cmath>
#include <boost/rational.hpp>

namespace utility
{
    const int DEFAULT_DENOMINATOR = 1000;    

    /**
    \brief 指定した有効桁数の有理数を得る
    \param v 有理数へ変換する値
    \param denominator 有効桁 (デフォルト 1000, 小数点第4位を四捨五入)
    \return 有理数
     */
    template<typename Integer=int>
    boost::rational<Integer> to_rational(double v, int denominator=DEFAULT_DENOMINATOR)
    {
        return boost::rational<Integer>(static_cast<Integer>(std::round(v * denominator)), denominator);
    }

    /**
    \brief threshole未満の値を0.0にまとめる
    \return threshole未満ならば0.0、それ以外はvalueを返す
     */
    inline double value_round(double value, double threshold)
    {
        return (std::abs(value) < threshold) ? 0.0 : value;
    }

}
