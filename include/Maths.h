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

/*



///always pass angles in radians for function in this namespace.
    ** Vectors have two common forms:
        (X, Y) - representing a point in 2d space.
        (A, L) - representing an angle and magnitude of motion.

        all functions in this namespace expect the (X, Y) form.
    *

    ///functions for converting between degrees and radians.
    double toDegrees(double);
    double toRadians(double);

    ///constants
    long double pi();
    double epsilon();
    long double phi();

    **@return
        -1: A < B
        0:  A == B
        1:  A > B
    *
    int8_t compare(double, double);

    ///@return the distance between two points in 2D space.
    *template <typename Ta, typename Tb>
    double distance(Vector2<Ta> a, Vector2<Tb> b) {
        return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    }*

    ///@return angle of (X, Y) form vector.
    template <typename T>
    double angle(Vector2<T> v) {
        return atan2(v.y, v.x);
    }

    ///@return angle of line between two points in 2D space.
    template <typename Ta, typename Tb>
    double angle(Vector2<Ta> a, Vector2<Tb> b) {
        return atan2(b.y - a.y, b.x - a.x);
    }

    ///@param (A) angle in radians
    ///@return unit vector representing A
    Vector2<double> toVector(double);

    **@param (R) the radical coefficient
    ***@param (N) the number to go under the radical
    ***@return N^(1/R)
    *
    double root(double, double);

    ///@return greatest common factor of both params.
    int64_t gcf(int64_t, int64_t);

    ///slope of a line between 2 points in 2D space.
    *template<typename T1, typename T2>
    double slope(Vector2<T1> a, Vector2<T2> b) {
        double slopeD = (b.y - a.y) / (b.x - a.x);
        return slopeD;
    }*/
    ///multiply Vector2 by Matrix
    /*template<typename T>
    const Matrix operator*(Matrix m, const Vector2<T> &v) {
        Matrix m2({(double)v.x}, {(double)v.y});
        return m * m2;
    }

};


*/
