#ifndef JML_H
#define JML_H

#ifndef __cplusplus
    #error This library requires the ISO 2011 C++ standard.
#else
    #if __cplusplus < 201103L
        #error This library requires the ISO 2011 C++ standard.
    #endif
#endif

#include "constants.h"
#include "Matrix.h"
#include "Angle.h"
#include "Fraction.hpp"
#include "trig.h"

namespace jml {

    template <
        typename T,
        typename U,
        typename = typename jutil::Enable<jutil::IsArithmatic<T>::Value>::Type,
        typename = typename jutil::Enable<jutil::IsArithmatic<U>::Value>::Type
    >
    int8_t compare(T a, U b) {
        double d = abs(static_cast<long double>(a) - static_cast<long double>(b));
        if (d < JML_EPSILON) {
            return JML_EQUAL;
        } else if (d > JML_EPSILON && a < b) {
            return JML_LESS;
        } else if (d > JML_EPSILON && a > b) {
            return JML_GREATER;
        } else {
            return -0x80;
        }
    }

    int64_t gcf(int64_t, int64_t);

    template <
        typename T,
        typename = typename jutil::Enable<jutil::IsArithmatic<T>::Value>::Type
    >
    constexpr T max(const T &a, const T &b) noexcept {
        return (compare(a, b) == JML_LESS? b : a);
    }

    template <
        typename T,
        typename = typename jutil::Enable<jutil::IsArithmatic<T>::Value>::Type
    >
    constexpr T min(const T &a, const T &b) noexcept {
        return (compare(a, b) == JML_GREATER? b : a);
    }

    /**

    @return Identity Matrix.
    @param U: Type of returned matrix.
    @param size: Rows and columns of returned matrix.

    */

    template<typename U, size_t size>
    Matrix<U, size, size> identity() {
        Matrix<U, size, size> result;
        for (size_t i = 0; i < size; ++i) {
            Vector<U, size> resultVec;
            resultVec[i] = static_cast<U>(1);
            result[i] = resultVec;
        }
        return result;
    }

    using Transformation = Matrix<long double, 4, 4>;
    using Vertex = Vector<long double, 4>;

    Transformation translate(const Vertex&, const Transformation& = identity<long double, 4>());

    Transformation scale(const Vertex&, const Transformation& = identity<long double, 4>());

    Transformation rotate(const Angle&, const Vector<int8_t, 3>&, const Transformation& = identity<long double, 4>());

    Transformation ortho(long double, long double, long double, long double, long double, long double);

    inline namespace literals {

        Angle operator "" _degs(long double);
        Angle operator "" _rads(long double);
        Angle operator "" _degs(unsigned long long);
        Angle operator "" _rads(unsigned long long);

        Fraction operator "" _f(long double);
        Fraction operator "" _f(unsigned long long);
    }
}

#endif // JML_H
