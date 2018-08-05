#include "Maths.h"
#include "trig.h"
#include <IO/IO.h>

enum {
    JML_TRIG_ODD = -1,
    JML_TRIG_EVEN = 1
};

namespace jml {

    const long double JAT_[] = {
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

    long double abs(long double z) {
        return (z < 0? z * -1 : z);
    }

    long double derivitive(long double(*f)(long double), long double a) {
        double a1 = a - JML_EPSILON;
        double a2 = a +  JML_EPSILON;
        double f1 = f(a1);
        double f2 = f(a2);
        return (f2 - f1) / (a2 - a1);
    }

    long double _ipow(long double n, long double nn, long e, long c) {
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

    long double _ln(long double z, long double nz, size_t e) {
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

    long double ln(long double z) {
        return _ln(z, 0, 0);
    }

    long double exp(long double z) {
        long double sum = 1 + z;
        long double n = 0;
        for (size_t i = 2; (n = pow(z, i) / factorial(i)) > JML_CUT; ++i) {
            sum += n;
        }
        return sum;
    }

    long double log(long double b, long double z) {
        return (ln(z) / ln(b));
    }

    long double log(long double z) {
        return log(JML_E, z);
    }

    long double root(long double n, long double r) {
        return pow(n, 1.0L / r);
    }

    long double _pow(long double a, long double b) {
        if (b < 0) {
            return 1.0L / _pow(a, -b);
        } else if (b == 0) {
            return 1.0L;
        } else {
            return exp(b * log(a));
        }
    }

    template <>
    long double pow<long double>(long double a, long double b) {
        return _pow(a, b);
    }

    template <>
    long double pow<double>(long double a, double b) {
        return _pow(a, b);
    }

    template <>
    long double pow<float>(long double a, float b) {
        return _pow(a, b);
    }

    long double sqrt(long double n) {
        return root(n, 2);
    }

    float sqrtf(float x) {
        const long double xhalf = 0.5f*x;

        union // get bits for floating value
        {
            float x;
            int i;
        } u;
        u.x = x;
        u.i = 0x5f3759df - (u.i >> 1);
        return x*u.x*(1.5f - xhalf*u.x*u.x);
    }

    jutil::Tuple<int64_t, long double> modf(long double z) {
        int64_t i = z;
        long double d = abs(z - i);
        return jutil::Tuple<int64_t, long double>(i, d);
    }

    long double round(long double z, uint8_t m) {
        return static_cast<long double>(static_cast<int64_t>(m == JML_ROUND_UP? z + 1 : z));
    }

    long double round(long double z) {
        long double d = jutil::get<1>(modf(z));
        return round(z, (d < 0.5L? JML_ROUND_DOWN : JML_ROUND_UP));
    }

    int16_t sign(long double z) {
        return (z < 0? -1 : 1);
    }

    long double fma(long double x, long double y, long double z) {
        return (x * y) + z;
    }

    long double copysign(long double x, long double y) {
        long double r = abs(x);
        r *= sign(y);
        return r;
    }

    long double factorial(long unsigned n) {
        long double r = 1;
        for (long unsigned i = n; i >= 1; --i) {
            r *= i;
        }
        return r;
    }
    constexpr long double e() {
        return JML_E;
    }

    long double _preptrig(long double a) {
        //a = abs(a);
        while (a > (2 * JML_PI)) {
            a -= 2 * JML_PI;
        }
        while (a < -(2 * JML_PI)) {
            a += 2 * JML_PI;
        }
        return a;
    }

    long double _trig(long double x, long double nx, size_t c, int16_t m) {
        size_t r = c * 2 - (m == JML_TRIG_EVEN? 0 : 1);
        long double d = (pow(x, r) / factorial(r)) * (c % 2 == 0? sign(m) : -1 * sign(m));
        if (abs(d) > JML_CUT) {
            return _trig(x, nx + d, c + 1, m);
        } else {
            return nx;
        }
    }

    long double _atrig(long double x, long double nx, size_t c) {
        long _2n = 2 * c;
        long _2np1 = _2n + 1;
        long double d = (factorial(_2n) / (pow(2, _2n) * pow(factorial(c), 2))) * (pow(x, _2np1) / _2np1);
        if (abs(d) > JML_CUT) {
            return _atrig(x, nx + d, c + 1);
        } else {
            return nx;
        }
    }

    long double cos(long double a) {
        a = _preptrig(a);
        return _trig(a, 1, 1, JML_TRIG_EVEN);
    }

    long double sin(long double a) {
        a = _preptrig(a);
        return _trig(a, a, 2, JML_TRIG_ODD);
    }

    long double asin(long double a) {
        return _atrig(a, 0, 0);
    }

    long double acos(long double a) {
        return JML_PIO2 - asin(a);
    }

    long double atan(long double x) {

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
                fma(-JAT_[0], s,JAT_[1]),
                q,
                fma(-JAT_[2], s, JAT_[3])
            ),
            o,
            fma (
                fma(-JAT_[4], s, JAT_[5]),
                q,
                fma(-JAT_[6], s, JAT_[7])
            )
        );
        p = fma(fma(fma(fma(fma(fma(fma(fma(fma(fma(fma(fma(p, s,
            -JAT_[8]),  s,
            JAT_[9]),   s,
            -JAT_[10]), s,
            JAT_[11]),  s,
            -JAT_[12]), s,
            JAT_[13]),  s,
            -JAT_[14]), s,
            JAT_[15]),  s,
            -JAT_[16]), s,
            JAT_[17]),  s,
            -JAT_[18]) * s, a, a
        );
        r = (z > 1.0? JAT_[19] - p : p);
        return copysign(r, x);
    }

    long double atan2(long double y, long double x) {
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

    long double tan(long double a) {
        return sin(a) / cos(a);
    }

    long double cot(long double a) {
        return 1.0L / tan(a);
    }

    long double acot(long double a) {
        return JML_PIO2 - atan(a);
    }

    long double sec(long double a) {
        return 1.0L / cos(a);
    }

    long double asec(long double a) {
        return acos(1.0L / a);
    }

    long double csc(long double a) {
        return 1.0L / sin(a);
    }

    long double acsc(long double a) {
        return JML_PIO2 - asec(a);
    }

    long double cos(const Angle &a) {
        return cos(Angle::radians(a));
    }

    long double sin(const Angle &a) {
        return sin(Angle::radians(a));
    }

    long double asin(const Angle &a) {
        return asin(Angle::radians(a));
    }

    long double acos(const Angle &a) {
        return acos(Angle::radians(a));
    }

    long double atan(const Angle &a) {
        return atan(Angle::radians(a));
    }

    long double tan(const Angle &a) {
        return tan(Angle::radians(a));
    }

    long double cot(const Angle &a) {
        return cot(Angle::radians(a));
    }

    long double acot(const Angle &a) {
        return acot(Angle::radians(a));
    }

    long double sec(const Angle &a) {
        return sec(Angle::radians(a));
    }

    long double asec(const Angle &a) {
        return asec(Angle::radians(a));
    }

    long double csc(const Angle &a) {
        return csc(Angle::radians(a));
    }

    long double acsc(const Angle &a) {
        return acsc(Angle::radians(a));
    }

    long double sigmoid(long double a) {
        long double ex = jml::pow(JML_E, a);
        return ex / (ex + 1);
    }

    long double tanh(long double a) {
        return 2 * sigmoid(2 * a) - 1;
    }

    long double sigmoid(const Angle &a) {
        return sigmoid(Angle::radians(a));
    }

    long double tanh(const Angle &a) {
        return tanh(Angle::radians(a));
    }

    double toDegrees(double rads) {
        return rads * 180.0L / JML_PI;
    }
    double toRadians(double degs) {
        return degs * JML_PI / 180.0L;
    }
    long double pi() {
        return JML_PI;
    }
    double epsilon() {
        return JML_EPSILON;
    }
    long double phi() {
        return JML_PHI;
    }
    int64_t gcf(int64_t a, int64_t b) {
        int l = a, r  = b, an = l % r;
        if (an == 0) return r;
        while (abs(an) > 0) {
        	l = an;
            an = r % an;
            r = l;
        }
        return l;
    }
    Angle literals::operator "" _degs(long double l) {
        long double rads = l * pi() / 180.0L;
        Angle a(rads);
        a.stringMode(Angle::DEGREES);
        return a;
    }
    Angle literals::operator "" _rads(long double l) {
        return Angle(l);
    }

    Angle literals::operator "" _degs(unsigned long long l) {
        long double rads = static_cast<long double>(l) * pi() / 180.0L;
        Angle a(rads);
        a.stringMode(Angle::DEGREES);
        return a;
    }
    Angle literals::operator "" _rads(unsigned long long l) {
        return Angle(static_cast<double>(l));
    }

    Fraction literals::operator "" _f(unsigned long long l) {
        return Fraction(l, 1);
    }

    Fraction literals::operator "" _f(long double l) {
        return Fraction(l);
    }

    Transformation translate(const Vertex &v, const Transformation &m) {
        Transformation t = {
            {1, 0, 0, v[0]},
            {0, 1, 0, v[1]},
            {0, 0, 1, v[2]},
            {0, 0, 0, 1},
        };

        return m * t;
    }

    Transformation scale(const Vertex &v, const Transformation &m) {
        Transformation t = m;
        t[0][0] *= v[0];
        t[1][1] *= v[1];
        t[2][2] *= v[2];
        return t;
    }

    Transformation rotate(const Angle &a, const Vector<int8_t, 3> &axes, const Transformation &m) {
        long double k[] = {Angle::radians(a) * axes[0], Angle::radians(a) * axes[1], Angle::radians(a) * axes[2]};
        Transformation x = {
            {1, 0, 0, 0},
            {0, cos(k[0]), -sin(k[0]), 0},
            {0, sin(k[0]), cos(k[0]), 0},
            {0, 0, 0, 1}
        };

        Transformation y = {
            {cos(k[1]), 0, sin(k[1]), 0},
            {0, 1, 0, 0},
            {-sin(k[1]), 0, cos(k[1]), 0},
            {0, 0, 0, 1}
        };

        Transformation z = {
            {cos(k[2]), -sin(k[2]), 0, 0},
            {sin(k[2]), cos(k[2]), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };

        return m * (z * y * x);
    }

    Transformation ortho(long double l, long double r, long double b, long double t, long double n, long double f) {
        Transformation result = identity<long double, 4>();
        result[0][0] = 2.0L / (r - l);
        result[1][1] = 2.0L / (t - b);
        result[2][2] = -(2.0L / (f - n));
        result[3][0] = -((r + l) / (r - l));
        result[3][1] = -((t + b) / (t - b));
        result[3][2] = -((f + n) / (f - n));
        return result;
    }

}
