#pragma once

#include <boost/math/constants/constants.hpp>
#include <geom_utl.hxx>

namespace utility
{
    /**
    \brief 角度を 0≦θ＜2π の範囲に正規化する。
    \param radian 角度(ラジアン)
    */
    inline
    double normalize_angle(double radian)
    {
        constexpr double two_pi = boost::math::constants::two_pi<double>();
        const auto r = std::fmod(radian, two_pi);
        return (r >= 0.0) ? r : (two_pi + r);
    }

    /**
    \brief 角度の差を計算する
    \details 第1引数の角度から第2引数の角度を引き、0≦θ＜2π に正規化する。
    \param a 引かれる角度(ラジアン)
    \param b 引く角度(ラジアン)
    */
    inline
    double diff_angle(double a, double b)
    {
        return normalize_angle(a - b);
    }

    /**
    \brief degreeに変換し、360°からの乖離が閾値未満の場合は丸めて0にする
    \brief 入力されたradianは内部で正規化されてdegree計算される
    */
    inline
    double radian_to_degree(double radian, double degree_tolerance = 0.0)
    {
        const auto normalize_radian = normalize_angle(radian);
        const auto degree = radians_to_degrees(normalize_radian);
        return std::abs(std::abs(degree) - 360.0) < degree_tolerance ? 0.0 : degree;
    }

    /**
    \brief 逆回りの角度 (2π - radian) を計算する
    \details
      (1) 引数を 0≦θ＜2π の範囲に正規化する
      (2) 2πから (1)を引く
      (3) (2)の結果を 0≦θ＜2π の範囲に正規化する
    \remark 戻り値は 0≦θ＜2π の範囲。
    \param radian 角度(ラジアン)
    */
    inline
    double reverse_angle(double radian)
    {
        const auto a = normalize_angle(radian);
        return normalize_angle(boost::math::constants::two_pi<double>() - a);
    }

    /**
    \brief 逆回りの角度 (2π - radian) を degree単位で返す
    \param radian 角度(ラジアン)
    */
    inline
    double radians_to_degrees_reverse(double radian)
    {
        return radian_to_degree(reverse_angle(radian));
    }

    /**
    \brief 逆回りの角度 (360°- degree) を radian単位で返す
    \param degree 角度(°)
    */
    inline
    double degrees_to_radians_reverse(double degree)
    {
        return reverse_angle(degrees_to_radians(degree));
    }
}
