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

#endif //MODERN_CPP_DEMO_MAX_HPP
