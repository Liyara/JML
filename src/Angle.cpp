#include "JML/Maths.h"
#include "JML/trig.h"

namespace jml {
    Angle::Angle() : raw(0.0L), sm(0) {}

    Angle::Angle(long double r) : raw(r), sm(0) {}

    Angle::Angle(const Angle &a) : raw(static_cast<long double>(a)), sm(a.sm) {}

    Angle::Angle(Angle &&a) : raw(static_cast<long double>(a)), sm(a.sm) {
        a.raw = 0;
        a.sm = 0;
    }

    Angle::Angle(AngularVector v) : sm(0) {
        setVector(v);
    }

    Angle &Angle::setRadians(long double r) {
        raw = r;
        return *this;
    }

    Angle &Angle::setDegrees(long double d) {
        raw = d * JML_PI/ 180.0L;
        return *this;
    }

    Angle &Angle::setVector(AngularVector v) {
        raw = atan2(v.y(), v.x());
        return *this;
    }

    auto Angle::getType() const -> Type {
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

    Angle Angle::bisector() const {
        return Angle(raw / 2.0L);
    }

    Angle Angle::inverse() const {
        long double r = abs(raw * 3.0L);
        while (compare(r, radians(360_degs)) == JML_GREATER) {
            r -= radians(360_degs);
        }
        return Angle(r);
    }

    Angle::operator long double() const {
        return raw;
    }

    Angle Angle::operator+(const Angle &b) const {
        return Angle(radians(*this) + radians(b));
    }
    Angle Angle::operator-(const Angle &b) const {
        return Angle(radians(*this) - radians(b));
    }
    Angle Angle::operator*(const Angle &b) const {
        return Angle(radians(*this) * radians(b));
    }
    Angle Angle::operator/(const Angle &b) const {
        return Angle(radians(*this) / radians(b));
    }

    Angle &Angle::operator+=(const Angle &b) {
        (*this) = (*this) + b;
        return *this;
    }
    Angle &Angle::operator-=(const Angle &b) {
        (*this) = (*this) - b;
        return *this;
    }
    Angle &Angle::operator*=(const Angle &b) {
        (*this) = (*this) * b;
        return *this;
    }
    Angle &Angle::operator/=(const Angle &b) {
        (*this) = (*this) / b;
        return *this;
    }

    bool Angle::operator>(const Angle &b) const {
        return compare(radians(*this), radians(b)) == JML_GREATER;
    }
    bool Angle::operator<(const Angle &b) const {
        return compare(radians(*this), radians(b)) == JML_LESS;
    }
    bool Angle::operator>=(const Angle &b) const {
        return (compare(radians(*this), radians(b)) == JML_GREATER) || (compare(radians(*this), radians(b)) == JML_EQUAL);
    }
    bool Angle::operator<=(const Angle &b) const {
        return (compare(radians(*this), radians(b)) == JML_LESS) || (compare(radians(*this), radians(b)) == JML_EQUAL);
    }
    bool Angle::operator==(const Angle &b) const {
        return compare(radians(*this), radians(b)) == JML_EQUAL;
    }
    bool Angle::operator!=(const Angle &b) const {
        return compare(radians(*this), radians(b)) != JML_EQUAL;
    }

    Angle &Angle::operator=(const Angle &a) {
        raw = static_cast<long double>(a);
        sm = a.sm;
        return *this;
    }

    Angle &Angle::operator=(Angle &&a) {
        raw = static_cast<long double>(a);
        sm = a.sm;
        a.raw = 0;
        a.sm = 0;
        return *this;
    }

    long double Angle::radians(const Angle &a) {
        return static_cast<long double>(a);
    }

    long double Angle::degrees(const Angle &a) {
        return radians(a) * 180.0L / JML_PI;
    }

    auto Angle::vector(const Angle &a) -> AngularVector {
        return AngularVector({cos(static_cast<long double>(a)), sin(static_cast<long double>(a))});
    }

    Angle &Angle::stringMode(uint8_t m) {
        sm = m;
        return *this;
    }

    Angle::operator jutil::String() {
        if (sm == RADIANS) {
            return jutil::String(radians(*this));
        } else {
            return jutil::String(degrees(*this));
        }
    }
    Angle::operator const jutil::String() const {
        if (sm == RADIANS) {
            return jutil::String(radians(*this));
        } else {
            return jutil::String(degrees(*this));
        }
    }

    Angle Angle::operator-() const {
        return Angle(raw * -1);
    }

    Angle::~Angle() {
        raw = 0;
        sm = 0;
    }

}
