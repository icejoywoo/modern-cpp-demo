//
// Created by icejoywoo on 2022/7/16.
//

#ifndef MODERN_CPP_DEMO_MAX_HPP
#define MODERN_CPP_DEMO_MAX_HPP

template <typename T>
inline T const& max (T const& a, T const& b)
{
    return a < b ? b : a;
}

template <typename T1, typename T2>
inline T1 const& max2 (T1 const& a, T2 const& b)
{
    return a < b ? b : a;
}

// RT 无法推算出来，调用的时候需要显示指定类型
template <typename RT, typename T1, typename T2>
inline RT const& max3 (T1 const& a, T2 const& b)
{
    return a < b ? b : a;
}

#endif //MODERN_CPP_DEMO_MAX_HPP
