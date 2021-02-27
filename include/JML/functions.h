#ifndef JML_FUNCTIONS_H
#define JML_FUNCTIONS_H

#include <JML/constants.h>

namespace jml {
    inline const long double *getJAT_() {
        static const long double JAT_[] = {
            0x1.53e1d2a25ff34p-16L,
            0x1.d3b63dbb65af4p-13L,
            0x1.312788dde0801p-10L,
            0x1.f9690c82492dbp-9L,
            0x1.2cf5aabc7cef3p-7L,
            0x1.162b0b2a3bfcep-6L,
            0x1.a7256feb6fc5cp-6L,
            0x1.171560ce4a483p-5L,
            0x1.4f44d841450e1p-5L,
            0x1.7ee3d3f36bb94p-5L,
            0x1.ad32ae04a9fd1p-5L,
            0x1.e17813d66954fp-5L,
            0x1.11089ca9a5bcdp-4L,
            0x1.3b12b2db51738p-4L,
            0x1.745d022f8dc5cp-4L,
            0x1.c71c709dfe927p-4L,
            0x1.2492491fa1744p-3L,
            0x1.99999999840d2p-3L,
            0x1.555555555544cp-2L,
            0x1.921fb54442d18p+0L
        };

        return JAT_;
    }

    inline long double abs(long double z) {
        return (z < 0? z * -1 : z);
    }

    template <
        typename T,
        typename U,
        typename = typename jutil::Enable<jutil::IsArithmatic<T>::Value>::Type,
        typename = typename jutil::Enable<jutil::IsArithmatic<U>::Value>::Type
    >
    int8_t compare(const T &a, const U &b) {
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

    template <
        typename T,
        typename = typename jutil::Enable<jutil::IsArithmatic<T>::Value>::Type
    >
    inline constexpr T max(const T &a, const T &b) noexcept {
        return (compare(a, b) == JML_LESS? b : a);
    }

    template <
        typename T,
        typename = typename jutil::Enable<jutil::IsArithmatic<T>::Value>::Type
    >
    inline constexpr T min(const T &a, const T &b) noexcept {
        return (compare(a, b) == JML_GREATER? b : a);
    }

    inline int16_t sign(long double z) {
        return (z < 0? -1 : 1);
    }

    inline long double fma(long double x, long double y, long double z) {
        return (x * y) + z;
    }

    inline long double copysign(long double x, long double y) {
        long double r = abs(x);
        r *= sign(y);
        return r;
    }

    inline long double factorial(long unsigned n) {
        long double r = 1;
        for (long unsigned i = n; i >= 1; --i) {
            r *= i;
        }
        return r;
    }
    inline constexpr long double e() {
        return JML_E;
    }

    inline long double _preptrig(long double a) {
        while (a > (2 * JML_PI)) {
            a -= 2 * JML_PI;
        }
        while (a < -(2 * JML_PI)) {
            a += 2 * JML_PI;
        }
        return a;
    }

    inline float sqrtf(float x) {
        const long double xhalf = 0.5f*x;

        union // get bits for floating value
        {
            float x;
            int i;
        } u;
        u.x = x;
        u.i = 0x5f300000 - (u.i >> 1);
        return x*u.x*(1.5f - xhalf*u.x*u.x);
    }

    inline long double derivitive(long double(*f)(long double), long double a) {
        double a1 = a - JML_EPSILON;
        double a2 = a +  JML_EPSILON;
        double f1 = f(a1);
        double f2 = f(a2);
        return (f2 - f1) / (a2 - a1);
    }

    inline long double round(long double z, uint8_t m) {
        return static_cast<long double>(static_cast<int64_t>(m == JML_ROUND_UP? z + 1 : z));
    }

     inline jutil::Tuple<int64_t, long double> modf(long double z) {
        int64_t i = z;
        long double d = abs(z - i);
        return jutil::Tuple<int64_t, long double>(i, d);
    }

    inline long double round(long double z) {
        long double d = jutil::get<1>(modf(z));
        return round(z, (d < 0.5L? JML_ROUND_DOWN : JML_ROUND_UP));
    }

    

    inline long double _ipow(long double n, long double nn, long e, long c) {
        if (e > 0) {
                while (c < e) {
                    nn *= n;
                    ++c;
                }
                return nn;
        } else if (e == 0) {
            return 1.0L;
        } else {
            return 1.0L / _ipow(n, nn, -e, c);
        }
    }

    long double root(long double, long double);

    inline long double sqrt(long double n) {
        return root(n, 2);
    }

    long double _ln(long double, long double, size_t);

    inline long double ln(long double z) {
        return _ln(z, 0, 0);
    }

    inline long double log(long double b, long double z) {
        return (ln(z) / ln(b));
    }

    inline long double log(long double z) {
        return log(JML_E, z);
    }

    long double exp(long double);

    inline long double _pow(long double a, long double b) {
        if (b < 0) {
            return 1.0L / _pow(a, -b);
        } else if (b == 0) {
            return 1.0L;
        } else {
            return exp(b * log(a));
        }
    }

    template <typename T>
    inline long double pow(long double n, T z) {
        return _ipow(n, n, static_cast<long>(z), 1);
    }

    template <>
    inline long double pow<long double>(long double a, long double b) {
        return _pow(a, b);
    }

    template <>
    inline long double pow<double>(long double a, double b) {
        return _pow(a, b);
    }

    template <>
    inline long double pow<float>(long double a, float b) {
        return _pow(a, b);
    }

    inline long double root(long double n, long double r) {
        return pow(n, 1.0L / r);
    }

    inline long double _ln(long double z, long double nz, size_t e) {
        long double t;
        uint64_t c;
        do {
            c = 2 * e + 1;
            t = 2.0L * ((1.0L / c) * pow((z - 1.0L) / (z + 1.0L), c));
            nz += t;
            ++e;
        } while (jml::abs(t) >= JML_EPSILON);
        return nz;
    }

    inline long double exp(long double z) {
        long double sum = 1 + z;
        long double n = 0;
        for (size_t i = 2; (n = pow(z, i) / factorial(i)) > JML_CUT; ++i) {
            sum += n;
        }
        return sum;
    }

    inline long double _trig(long double x, long double nx, size_t c, int16_t m) {
        size_t r = c * 2 - (m == JML_TRIG_EVEN? 0 : 1);
        long double d = (pow(x, r) / factorial(r)) * (c % 2 == 0? sign(m) : -1 * sign(m));
        if (abs(d) > JML_CUT) {
            return _trig(x, nx + d, c + 1, m);
        } else {
            return nx;
        }
    }

    inline long double _atrig(long double x, long double nx, size_t c) {
        long _2n = 2 * c;
        long _2np1 = _2n + 1;
        long double d = (factorial(_2n) / (pow(2, _2n) * pow(factorial(c), 2))) * (pow(x, _2np1) / _2np1);
        if (abs(d) > JML_CUT) {
            return _atrig(x, nx + d, c + 1);
        } else {
            return nx;
        }
    }

    inline long double cos(long double a) {
        a = _preptrig(a);
        return _trig(a, 1, 1, JML_TRIG_EVEN);
    }

    inline long double sin(long double a) {
        a = _preptrig(a);
        return _trig(a, a, 2, JML_TRIG_ODD);
    }

    inline long double asin(long double a) {
        return _atrig(a, 0, 0);
    }

    inline long double acos(long double a) {
        return JML_PIO2 - asin(a);
    }

    inline long double atan(long double x) {

        if (x < 0) {
            return -atan(abs(x));
        }

        long double a, z, p, r, s, q, o;
        z = abs(x);
        a = (z > 1.0L? 1.0L / z : z);
        s = a * a;
        q = s * s;
        o = q * q;
        p =
        fma (
            fma (
                fma(-getJAT_()[0], s,getJAT_()[1]),
                q,
                fma(-getJAT_()[2], s, getJAT_()[3])
            ),
            o,
            fma (
                fma(-getJAT_()[4], s, getJAT_()[5]),
                q,
                fma(-getJAT_()[6], s, getJAT_()[7])
            )
        );
        p = fma(fma(fma(fma(fma(fma(fma(fma(fma(fma(fma(fma(p, s,
            -getJAT_()[8]),  s,
            getJAT_()[9]),   s,
            -getJAT_()[10]), s,
            getJAT_()[11]),  s,
            -getJAT_()[12]), s,
            getJAT_()[13]),  s,
            -getJAT_()[14]), s,
            getJAT_()[15]),  s,
            -getJAT_()[16]), s,
            getJAT_()[17]),  s,
            -getJAT_()[18]) * s, a, a
        );
        r = (z > 1.0? getJAT_()[19] - p : p);
        return copysign(r, x);
    }

    inline long double atan2(long double y, long double x) {
        if (x > 0) {
            return atan(y / x);
        } else if (x < 0 && y >= 0) {
            return atan(y / x) + JML_PI;
        } else if (x < 0  && y < 0) {
            return atan(y / x) - JML_PI;
        } else if (x == 0 && y > 0) {
            return JML_PIO2;
        } else if (x == 0 && y < 0) {
            return -JML_PIO2;
        } else {
            return 0;
        }
    }

    inline long double tan(long double a) {
        return sin(a) / cos(a);
    }

    inline long double cot(long double a) {
        return 1.0L / tan(a);
    }

    inline long double acot(long double a) {
        return JML_PIO2 - atan(a);
    }

    inline long double sec(long double a) {
        return 1.0L / cos(a);
    }

    inline long double asec(long double a) {
        return acos(1.0L / a);
    }

    inline long double csc(long double a) {
        return 1.0L / sin(a);
    }

    inline long double acsc(long double a) {
        return JML_PIO2 - asec(a);
    }

    inline long double sigmoid(long double a) {
        long double ex = jml::pow(JML_E, a);
        return ex / (ex + 1);
    }

    inline long double tanh(long double a) {
        return 2 * sigmoid(2 * a) - 1;
    }

    inline double toDegrees(double rads) {
        return rads * 180.0L / JML_PI;
    }
    inline double toRadians(double degs) {
        return degs * JML_PI / 180.0L;
    }
    inline long double pi() {
        return JML_PI;
    }
    inline double epsilon() {
        return JML_EPSILON;
    }
    inline long double phi() {
        return JML_PHI;
    }
    inline int64_t gcf(int64_t a, int64_t b) {
        int l = a, r  = b, an = l % r;
        if (an == 0) return r;
        while (abs(an) > 0) {
        	l = an;
            an = r % an;
            r = l;
        }
        return l;
    }

    inline long double fmod(long double b, long double m) {
        long double z = b / m;
        return (z - (int64_t)z) * m;
    }
}

#endif