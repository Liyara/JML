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

#include "JML/Vector.hpp"

namespace jml {
    class Angle : public jutil::StringInterface, public jutil::FloatingPoint<Angle> {

    /**

    @brief (X, Y) form vector type to be used with angles.

    */

    using AngularVector = Vector<long double, 2>;

    public:

        /**

        @brief Enum describing all possible angle types.

        */

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

        Angle &stringMode(uint8_t);

        /**

        @brief Constructor which sets angle at 0.

        */

        Angle();

        /**

        @brief Explicit constrcutor which takes radians.

        */

        explicit Angle(long double);

        /**

        @brief Copy constructor.

        */

        Angle(const Angle&);
        Angle(Angle&&);

        /**

        @brief Constructor which sets the angle as the atan of a vector.

        */

        Angle(AngularVector);

        /**

        @brief Set the angle in radians.

        */

        Angle &setRadians(long double);

        /**

        @brief Set the angle in degrees. (parameter is internally converted into radians.)

        */

        Angle &setDegrees(long double);

        /**

        @brief Set the angle using a vector; the angle becomes the atan of the vector.

        */

        Angle &setVector(AngularVector);

        /**

        @return The type of the angle.

        */

        Type getType() const;

        /**

        @return The bisector of the angle.

        */

        Angle bisector() const;

        /**

        @return The inverse of the angle.

        */

        Angle inverse() const;

        /**

        @brief Implicit type conversion into double. Resulting double is always in radians.

        */

        explicit operator long double() const;

        /**

        @section    OPERATORS.
        @brief  These basic operators are written between angles only.
                However, keep in mind that an explicit conversion between Angles and doubles has been defined.

        */

        Angle operator+(const Angle&) const;
        Angle operator-(const Angle&) const;
        Angle operator*(const Angle&) const;
        Angle operator/(const Angle&) const;

        Angle &operator+=(const Angle&);
        Angle &operator-=(const Angle&);
        Angle &operator*=(const Angle&);
        Angle &operator/=(const Angle&);
        Angle &operator=(const Angle&);
        Angle &operator=(Angle&&);

        bool operator>(const Angle&) const;
        bool operator<(const Angle&) const;
        bool operator>=(const Angle&) const;
        bool operator<=(const Angle&) const;
        bool operator==(const Angle&) const;
        bool operator!=(const Angle&) const;

        Angle operator-() const;

        /**

        @return Conversion of angle into double as radians.

        */

        static long double radians(const Angle&);

        /**

        @brief Conversion of angle into double as degrees.

        */

        static long double degrees(const Angle&);

        /**

        @brief Conversion of angle into vector as (X, Y).

        */

        static AngularVector vector(const Angle&);

        operator jutil::String();
        operator const jutil::String() const;

        virtual ~Angle();

    private:
        long double raw;
        uint8_t sm;
    };
}

#endif // JML_ANGLE_H
