//
// Created by Matq on 16/01/2025.
//

#pragma once

namespace Numbers {
template<class T>
T lerp(T a, T b, double t) {
    return a + t * (b - a);
}

template<class T>
T mapValue(T x, T in_min, T in_max, T out_min, T out_max) {
    T t = static_cast<double>(x - in_min) / static_cast<double>(in_max - in_min);
    return lerp(out_min, out_max, t);
}

}
