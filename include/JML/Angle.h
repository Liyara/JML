#ifndef JML_ANGLE_H
#define JML_ANGLE_H

/**

@file       Angle.h
@author     Jarrett W. Kastler (Liyara)
@date       12/2/2016
@version    2.0

@section    DESCRIPTION
Implements an Angle class designed to mimic the behaviour of mathematical angles.

*/

#include <JML/Vector.hpp>

namespace jml {

    class Angle;

    inline namespace literals {
        Angle operator "" _degs(long double);
        Angle operator "" _rads(long double);
        Angle operator "" _degs(unsigned long long);
        Angle operator "" _rads(unsigned long long);
    }

    class Angle : public jutil::StringInterface, public jutil::FloatingPoint<Angle> {

    /**

    @brief (X, Y) form vector type to be used with angles.

    */

    using AngularVector = Vector<long double, 2>;

    public:

         enum Type {
            ACUTE,
            RIGHT,
            OBTUSE,
            STRAIGHT,
            REFLEX
        };

        enum {
            RADIANS,
            DEGREES
        };

        Angle() : raw(0.0L), sm(0) {}

        Angle(long double r) : raw(r), sm(0) {}

        Angle(const Angle &a) : raw(static_cast<long double>(a)), sm(a.sm) {}

        Angle(Angle &&a) : raw(static_cast<long double>(a)), sm(a.sm) {
            a.raw = 0;
            a.sm = 0;
        }

        Angle(AngularVector v) : sm(0) {
            setVector(v);
        }

        Angle &setRadians(long double r) {
            raw = r;
            return *this;
        }

        Angle &setDegrees(long double d) {
            raw = d * JML_PI/ 180.0L;
            return *this;
        }

        Angle &setVector(AngularVector v) {
            raw = atan2(v.y(), v.x());
            return *this;
        }

        auto getType() const -> Type {
            long double degs = abs(degrees(*this));
            while (compare(degs, 360.0L) == JML_GREATER) {
                degs -= 360.0L;
            }
            if (compare(degs, 90.0L) == JML_LESS) {
                return Type::ACUTE;
            } else if (compare(degs, 90.0L) == JML_EQUAL) {
                return Type::RIGHT;
            } else if ((compare(degs, 90.0L) == JML_GREATER) && (compare(degs, 180.0L) == JML_LESS)) {
                return Type::OBTUSE;
            } else if (compare(degs, 180.0L) == JML_EQUAL) {
                return Type::STRAIGHT;
            } else {
                return Type::REFLEX;
            }
        }

        Angle bisector() const {
            return Angle(raw / 2.0L);
        }

        Angle inverse() const {
            long double r = abs(raw * 3.0L);
            while (compare(r, radians(360_degs)) == JML_GREATER) {
                r -= radians(360_degs);
            }
            return Angle(r);
        }

        explicit operator long double() const {
            return raw;
        }

        Angle operator+(const Angle &b) const {
            return Angle(radians(*this) + radians(b));
        }
        Angle operator-(const Angle &b) const {
            return Angle(radians(*this) - radians(b));
        }
        Angle operator*(const Angle &b) const {
            return Angle(radians(*this) * radians(b));
        }
        Angle operator/(const Angle &b) const {
            return Angle(radians(*this) / radians(b));
        }

        Angle &operator+=(const Angle &b) {
            (*this) = (*this) + b;
            return *this;
        }
        Angle &operator-=(const Angle &b) {
            (*this) = (*this) - b;
            return *this;
        }
        Angle &operator*=(const Angle &b) {
            (*this) = (*this) * b;
            return *this;
        }
        Angle &operator/=(const Angle &b) {
            (*this) = (*this) / b;
            return *this;
        }

        bool operator>(const Angle &b) const {
            return compare(radians(*this), radians(b)) == JML_GREATER;
        }
        bool operator<(const Angle &b) const {
            return compare(radians(*this), radians(b)) == JML_LESS;
        }
        bool operator>=(const Angle &b) const {
            return (compare(radians(*this), radians(b)) == JML_GREATER) || (compare(radians(*this), radians(b)) == JML_EQUAL);
        }
        bool operator<=(const Angle &b) const {
            return (compare(radians(*this), radians(b)) == JML_LESS) || (compare(radians(*this), radians(b)) == JML_EQUAL);
        }
        bool operator==(const Angle &b) const {
            return compare(radians(*this), radians(b)) == JML_EQUAL;
        }
        bool operator!=(const Angle &b) const {
            return compare(radians(*this), radians(b)) != JML_EQUAL;
        }

        Angle &operator=(const Angle &a) {
            raw = static_cast<long double>(a);
            sm = a.sm;
            return *this;
        }

        Angle &operator=(Angle &&a) {
            raw = static_cast<long double>(a);
            sm = a.sm;
            a.raw = 0;
            a.sm = 0;
            return *this;
        }

        static long double radians(const Angle &a) {
            return static_cast<long double>(a);
        }

        static long double degrees(const Angle &a) {
            return radians(a) * 180.0L / JML_PI;
        }

        static auto vector(const Angle &a) -> AngularVector {
            return AngularVector({cos(static_cast<long double>(a)), sin(static_cast<long double>(a))});
        }

        Angle &stringMode(uint8_t m) {
            sm = m;
            return *this;
        }

        operator jutil::String() {
            if (sm == RADIANS) {
                return jutil::String(radians(*this));
            } else {
                return jutil::String(degrees(*this));
            }
        }
        operator const jutil::String() const {
            if (sm == RADIANS) {
                return jutil::String(radians(*this));
            } else {
                return jutil::String(degrees(*this));
            }
        }

        Angle operator-() const {
            return Angle(raw * -1);
        }

        ~Angle() {
            raw = 0;
            sm = 0;
        }


    private:
        long double raw;
        uint8_t sm;
    };

    inline long double cos(const Angle &a) {
        return cos(Angle::radians(a));
    }

    inline long double sin(const Angle &a) {
        return sin(Angle::radians(a));
    }

    inline long double asin(const Angle &a) {
        return asin(Angle::radians(a));
    }

    inline long double acos(const Angle &a) {
        return acos(Angle::radians(a));
    }

    inline long double atan(const Angle &a) {
        return atan(Angle::radians(a));
    }

    inline long double tan(const Angle &a) {
        return tan(Angle::radians(a));
    }

    inline long double cot(const Angle &a) {
        return cot(Angle::radians(a));
    }

    inline long double acot(const Angle &a) {
        return acot(Angle::radians(a));
    }

    inline long double sec(const Angle &a) {
        return sec(Angle::radians(a));
    }

    inline long double asec(const Angle &a) {
        return asec(Angle::radians(a));
    }

    inline long double csc(const Angle &a) {
        return csc(Angle::radians(a));
    }

    inline long double acsc(const Angle &a) {
        return acsc(Angle::radians(a));
    }

    inline long double sigmoid(const Angle &a) {
        return sigmoid(Angle::radians(a));
    }

    inline long double tanh(const Angle &a) {
        return tanh(Angle::radians(a));
    }

    inline Angle literals::operator "" _degs(long double l) {
        long double rads = l * pi() / 180.0L;
        Angle a(rads);
        a.stringMode(Angle::DEGREES);
        return a;
    }
    inline Angle literals::operator "" _rads(long double l) {
        return Angle(l);
    }

    inline Angle literals::operator "" _degs(unsigned long long l) {
        long double rads = static_cast<long double>(l) * pi() / 180.0L;
        Angle a(rads);
        a.stringMode(Angle::DEGREES);
        return a;
    }
    inline Angle literals::operator "" _rads(unsigned long long l) {
        return Angle(static_cast<double>(l));
    }
}

#endif // JML_ANGLE_H
