#include "Fraction.hpp"
#include "Maths.h"
#include "misc.h"

namespace jml {


    Fraction::Fraction() : _numerator(1), _denominator(1), neg(false) {}

    Fraction::Fraction(const Fraction &other) :   _numerator(other._numerator), _denominator(other._denominator), neg(other.neg) {}

    Fraction::Fraction(Fraction &&other) :  _numerator(other._numerator), _denominator(other._denominator), neg(other.neg){
        other._numerator = 0;
        other._denominator = 0;
    }

    Fraction::Fraction(int64_t a, int64_t b) : _numerator(abs(a)), _denominator(abs(b)) {
        if ((a < 0 && b >= 0) || (b < 0 && a >= 0)) {
            neg = true;
        } else {
            neg = false;
        }
    }

    Fraction::Fraction(long double b) {
        if (b < 0) {
            neg = true;
        } else {
            neg = false;
        }
        b = abs(b);
        jutil::String s(b);
        size_t pos = 0, l = s.size();
        s.find('.', &pos);
        if (pos == s.size()) {
            Fraction r(b, 1);
            *this = r;
        }
        while (true) {
            s = s.substr(1, l - 1);
            l = s.size();
            if (s[0] == '.') {
                break;
            }
        }
        uint64_t m = round(pow(10, l));
        auto sep = modf(b);
        uint64_t d = m;
        long double md = jutil::get<JML_DECIMAL_PART>(sep);
        if (b > 1) {
            d = round(d * md);
        } else {
            d = round(d * b);
        }
        Fraction f(d, m);
        f.neg = neg;
        if (b > 1) {
            f.numerator((static_cast<uint64_t>(b) * f._denominator) + f._numerator);
        }
        *this = f.simplify();
    }

    Fraction Fraction::reciprocal() {
        Fraction r(_denominator, _numerator);
        r.neg = neg;
        return r;
    }

    bool Fraction::negative() const {
        return neg;
    }

    Fraction &Fraction::negative(bool b) {
        neg = b;
        return *this;
    }

    uint64_t Fraction::numerator() const {
        return _numerator;
    }

    uint64_t Fraction::denominator() const {
        return _denominator;
    }

    auto Fraction::numerator(const uint64_t &v) -> Fraction& {
        _numerator = v;
        simplify();
        return *this;
    }

    auto Fraction::denominator(const uint64_t &v) -> Fraction& {
        _denominator = v;
        simplify();
        return *this;
    }

    Fraction &Fraction::simplify() {
        uint64_t g = 0;
        g = gcf(_numerator, _denominator);
        _numerator /= g;
        _denominator /= g;
        return *this;
    }

    Fraction Fraction::operator+(const Fraction &b) const {
        Fraction l = *this;
        l._numerator *= b._denominator;
        l._denominator *= b._denominator;
        Fraction r  = b;
        r._numerator *= _denominator;
        r._denominator *= _denominator;
        Fraction result;
        if ((l.neg && r.neg) || (!l.neg && !r.neg)) {
            result._numerator = l._numerator + r._numerator;
        } else {
            result._numerator = abs(static_cast<int64_t>(l._numerator) - static_cast<int64_t>(r._numerator));
            result.neg = true;
        }
        result._denominator = l._denominator;
        return result.simplify();
    }
    Fraction Fraction::operator-(const Fraction &b) const {
        return (*this) + (b * -1);
    }
    Fraction Fraction::operator/(const Fraction &b) const {
        Fraction r = Fraction(b._denominator, b._numerator);
        r.neg = b.neg;
        return *this * r;
    }
    Fraction Fraction::operator*(const Fraction &b) const {
        Fraction r(_numerator * b._numerator, _denominator * b._denominator);
        if ((b.neg && neg) || (!b.neg && !neg)) {
            r.neg = false;
        } else {
            r.neg = true;
        }
        return r.simplify();
    }
    Fraction &Fraction::operator+=(const Fraction &b) {
        *this = *this + b;
        return *this;
    }
    Fraction &Fraction::operator-=(const Fraction &b) {
        *this = *this - b;
        return *this;
    }
    Fraction &Fraction::operator/=(const Fraction &b) {
        *this = *this / b;
        return *this;
    }
    Fraction &Fraction::operator*=(const Fraction &b) {
        *this = *this * b;
        return *this;
    }
    Fraction &Fraction::operator=(const Fraction &b) {
        _numerator = b._numerator;
        _denominator = b._denominator;
        neg = b.neg;
        return *this;
    }
    Fraction &Fraction::operator=(Fraction &&f) {
        _numerator = f._numerator;
        _denominator = f._denominator;
        neg = f.neg;
        f._numerator = 0;
        f._denominator = 0;
        return *this;
    }
    Fraction::operator long double() const {
        long double r = static_cast<long double>(_numerator) / static_cast<long double>(_denominator);
        r *= (neg? -1 : 1);
        return r;
    }
    bool Fraction::operator==(const Fraction &b) const {
        Fraction a = *this, f = b;
        return (a._numerator == f._numerator && a._denominator == f._denominator && a.neg == f.neg);
    }
    bool Fraction::operator!=(const Fraction &b) const {
        return !(*this == b);
    }
    bool Fraction::operator>(const Fraction &b) const {
        return (compare(static_cast<long double>(*this), static_cast<long double>(b)) == JML_GREATER);
    }
    bool Fraction::operator<(const Fraction &b) const {
        return (compare(static_cast<long double>(*this), static_cast<long double>(b)) == JML_LESS);
    }
    bool Fraction::operator>=(const Fraction &b) const {
        int8_t c = compare(static_cast<long double>(*this), static_cast<long double>(b));
        return (c == JML_GREATER || c == JML_LESS);
    }
    bool Fraction::operator<=(const Fraction &b) const {
        int8_t c = compare(static_cast<long double>(*this), static_cast<long double>(b));
        return (c == JML_LESS || c == JML_EQUAL);
    }

    ///+
    Fraction Fraction::operator+(long double d) const {
        Fraction f(d);
        return *this + f;
    }

    ///-
    Fraction Fraction::operator-(long double d) const {
        Fraction f(d);
        return *this - f;
    }
    ///divide
    Fraction Fraction::operator/(long double d) const {
        Fraction f(d);
        return *this / f;
    }

    ///*
    Fraction Fraction::operator*(long double d) const {
        Fraction f(d);
        return *this * f;
    }

    ///+=
    Fraction &Fraction::operator+=(long double d) {
        Fraction f(d);
        *this += f;
        return *this;
    }

    ///-=
    Fraction &Fraction::operator-=(long double d) {
        Fraction f(d);
        *this -= f;
        return *this;
    }

    ///divide=
    Fraction &Fraction::operator/=(long double d) {
        Fraction f(d);
        *this /= f;
        return *this;
    }

    ///*=
    Fraction &Fraction::operator*=(long double d) {
        Fraction f(d);
        *this *= f;
        return *this;
    }

    ///=
    Fraction &Fraction::operator=(long double d) {
        Fraction f(d);
        *this = f;
        return *this;
    }

    ///==
    bool Fraction::operator==(long double d) const {
        Fraction f(d);
        return *this == f;
    }

    ///!=
    bool Fraction::operator!=(long double d) const {
        Fraction f(d);
        return *this != f;
    }

    ///>
    bool Fraction::operator>(long double d) const {
        Fraction f(d);
        return *this > f;
    }

    ///<
    bool Fraction::operator<(long double d) const {
        Fraction f(d);
        return *this < f;
    }

    ///>=
    bool Fraction::operator>=(long double d) const {
        Fraction f(d);
        return *this >= f;
    }

    ///<=
    bool Fraction::operator<=(long double d) const {
        Fraction f(d);
        return *this <= f;
    }

    Fraction::operator jutil::String() {
        jutil::String r = "";
        if (neg) {
            r.insert('-');
        }
        if (_denominator != 1) {
            r += "(";
            r += jutil::String::toString(_numerator);
            r += " / ";
            r += jutil::String::toString(_denominator);
            r += ")";
        } else {
            r += jutil::String::toString(_numerator);
        }
        return r;
    }
    Fraction::operator const jutil::String() const {
        jutil::String r = "";
        if (neg) {
            r.insert('-');
        }
        if (_denominator != 1) {
            r += "(";
            r += jutil::String::toString(_numerator);
            r += " / ";
            r += jutil::String::toString(_denominator);
            r += ")";
        } else {
            r += jutil::String::toString(_numerator);
        }
        return r;
    }

    Fraction Fraction::operator-() const {
        Fraction f(_numerator, _denominator);
        f.negative((neg? false : true));
        return f;
    }

    Fraction::~Fraction() {}

}
