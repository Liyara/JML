#ifndef FRACTION_HPP_INCLUDED
#define FRACTION_HPP_INCLUDED

///only represents non-mixed fractions!

#include <Core/integers.h>
#include <Core/size_t.h>
#include <Core/String.h>
#include <Traits/IsFloatingPoint.hpp>

namespace jml {
    class Fraction : public jutil::StringInterface, public jutil::FloatingPoint<Fraction> {

    public:
        ///ctor
        Fraction(int64_t, int64_t);
        explicit Fraction(long double);
        Fraction();

        ///copy ctor
        Fraction(const Fraction&);
        Fraction(Fraction&&);

        ///@return Fraction(denominator / numerator)
        Fraction reciprocal();

        ///simplifies fraction
        Fraction &simplify();

        explicit operator long double() const;

        uint64_t numerator() const;
        uint64_t denominator() const;
        bool negative() const;

        Fraction &numerator(const uint64_t&);
        Fraction &denominator(const uint64_t&);
        Fraction &negative(bool);

        /**operator overloads (all operations are simplified)
        -----------------------------------------------------------------------------------------------------------------------------**/
        ///+
        Fraction operator+(const Fraction&) const;

        ///-
        Fraction operator-(const Fraction&) const;

        ///divide
        Fraction operator/(const Fraction&) const;

        ///*
        Fraction operator*(const Fraction&) const;

        ///+=
        Fraction &operator+=(const Fraction&);

        ///-=
        Fraction &operator-=(const Fraction&);

        ///divide=
        Fraction &operator/=(const Fraction&);

        ///*=
        Fraction &operator*=(const Fraction&);

        ///=
        Fraction &operator=(const Fraction&);
        Fraction &operator=(Fraction&&);

        ///==
        bool operator==(const Fraction&) const;

        ///!=
        bool operator!=(const Fraction&) const;
        ///>
        bool operator>(const Fraction&) const;

        ///<
        bool operator<(const Fraction&) const;

        ///>=
        bool operator>=(const Fraction&) const;

        ///<=
        bool operator<=(const Fraction&) const;

        ///+
        Fraction operator+(long double) const;

        ///-
        Fraction operator-(long double) const;

        ///divide
        Fraction operator/(long double) const;

        ///*
        Fraction operator*(long double) const;

        ///+=
        Fraction &operator+=(long double);

        ///-=
        Fraction &operator-=(long double);

        ///divide=
        Fraction &operator/=(long double);

        ///*=
        Fraction &operator*=(long double);

        ///=
        Fraction &operator=(long double);

        ///==
        bool operator==(long double) const;

        ///!=
        bool operator!=(long double) const;

        ///>
        bool operator>(long double) const;

        ///<
        bool operator<(long double) const;

        ///>=
        bool operator>=(long double) const;

        ///<=
        bool operator<=(long double) const;
        ///---------------------------------------------------------------------------------------------------------------------------

        operator jutil::String();
        operator const jutil::String() const;

        Fraction operator-() const;

        virtual ~Fraction();

    private:
        uint64_t _numerator, _denominator;
        bool neg;
    };
}

#endif // FRACTION_HPP_INCLUDED
