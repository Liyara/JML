#ifndef JML_VECTOR_H
#define JML_VECTOR_H

/**

@file       Vector.hpp
@author     Jarrett W. Kastler (Liyara)
@date       12/1/2016
@version    2.0

@section    DESCRIPTION
Implements a Vector class designed to mimic the behaviour of mathematical Vectors.

*/

#include <Container/Queue.hpp>
#include <Traits/traits.hpp>
#include <Core/String.h>
#include <initializer_list>

#include "log.h"
#include "IO.h"

namespace jml {

    ///Forward-declare Matrix. @see Matrix.hpp
    template<
        typename T,
        size_t rows,
        size_t cols,
        typename = typename jutil::Enable<jutil::IsArithmatic<T>::Value>::Type
    >
    class Matrix;

    /**

    @param T: Base numerical type to be stored in the vector.
    @param length: Number of elements in the vector.

    */

    template<
        typename T,
        size_t length,
        typename = typename jutil::Enable<jutil::IsArithmatic<T>::Value>::Type
    >
    class Vector : public jutil::StringInterface {

    public:

        typedef std::initializer_list<T> Literal;
        typedef jutil::Queue<T> Raw;
        typedef T* Iterator;
        typedef const T* ConstIterator;
        typedef T type;

        /**

        @brief A number of methods designed to access elemtns of the vector using conventional aliases.
        @return Element at given position.

        */

        T &x() {return (*this)[0];}
        T &y() {return (*this)[1];}
        T &z() {return (*this)[2];}
        T &w() {return (*this)[3];}

        T &r() {return (*this)[0];}
        T &g() {return (*this)[1];}
        T &b() {return (*this)[2];}
        T &a() {return (*this)[3];}

        const T &x() const {return (*this)[0];}
        const T &y() const {return (*this)[1];}
        const T &z() const {return (*this)[2];}
        const T &w() const {return (*this)[3];}

        const T &r() const {return (*this)[0];}
        const T &g() const {return (*this)[1];}
        const T &b() const {return (*this)[2];}
        const T &a() const {return (*this)[3];}

        void array(T arr[length]) {
            for (size_t i = 0; i < length; ++i) {
                arr[i] = (*this)[i];
            }
        }

        /**

        @return Element at position @param i.

        */

        T get(size_t i) const {
            return rawVector[i];
        }

        /**

        @return Reference to element at position @param i.

        */

        T &operator[](size_t i) {
            return rawVector[i];
        }

        const T &operator[](size_t i) const {
            return rawVector[i];
        }

        /**

        @return Number of elements in vector.

        */

        constexpr size_t getLength() const {
            return length;
        }

        /**

        @brief Constrcutor which sets all elements to 0.

        */

        Vector() {
            rawVector.reserve(length);
            for (size_t i = 0; i < length; ++i) {
                rawVector.insert(static_cast<T>(0));
            }
        }

        /**

        @brief Constructor which sets all elements to the corresponsing element in @param v.

        */

        Vector(Literal v) {
            rawVector.reserve(length);
            for (size_t i = 0; i < length; ++i) {
                if (i < v.size()) {
                    rawVector.insert(static_cast<T>(*(v.begin() + i)));
                } else if (i >= v.size()) {
                    rawVector.insert(static_cast<T>(0));
                }
            }
        }

        /**

        @brief Constructor with sets all elements to @param n.

        */

        Vector(T n) {
            rawVector.reserve(length);
            for (size_t i = 0; i < length; ++i) {
                rawVector.insert(n);
            }
        }

        /**

        @brief Copy constrctor.
        */

        template <
            typename U,
            size_t len,
            typename... Args,
            typename = typename jutil::Enable<jutil::Convert<U, T>::Value>::Type
        >
        Vector(const Vector<U, len> &v, const Args... args) {
            rawVector.reserve(length);
            T arr[] = {static_cast<T>(args)...};
            for (size_t i = 0; i < length; ++i) {
                if (i < len) {
                    rawVector.insert(static_cast<T>(v.get(i)));
                } else {
                    rawVector.insert(arr[i - len]);
                }
            }
        }

        ///@return *this / ||*this||
        Vector<long double, length> unitForm() const {
            Vector<long double, length> v;
            size_t index = 0;
            for (auto &i: rawVector) {
                v[index] = (i / magnitude());
                ++index;
            }
            return v;
        }

        ///@return ||*this||
        long double magnitude() const {
            long double r = 0.0L;
            for (auto &i: rawVector) {
                r += pow(i, 2);
            }
            long double s = static_cast<long double>(sqrtf(static_cast<float>(r)));
            return s;
        }

        /**

        @return Pointer to the first element in the vector.

        */

        Iterator begin() {
            return &((*this)[0]);
        }

        /**

        @return Const pointer to the first element in the vector.

        */

        ConstIterator begin() const {
            return &((*this)[0]);
        }

        /**

        @return Pointer to the last element in the vector.

        */

        Iterator end() {
            return (&((*this)[length - 1])) + 1;
        }

        /**

        @return Const pointer to the last element in the vector.

        */

        ConstIterator end() const {
            return (&((*this)[length - 1])) + 1;
        }

        /**

        @brief Addition operator between two vectors.

        */

        template <typename U>
        auto operator+(const Vector<U, length> &b) const -> Vector<ADD_T(T, U), length> {
            typedef Vector<ADD_T(T, U), length> rType;
            rType result;
            for (size_t i = 0; i < length; ++i) {
                result[i] = static_cast<ADD_T(T, U)>(this->get(i)) + static_cast<ADD_T(T, U)>(b.get(i));
            }
            return result;
        }

        /**

        @brief Subtraction operator between two vectors.

        */

        template <typename U>
        auto operator-(const Vector<U, length> &b) const -> Vector<SUBTRACT_T(T, U), length> {
            return *this + (b * -1);
        }

        /**

        @brief Division operator between the vector and a scalar

        */

        template <typename U, typename = typename jutil::Enable<jutil::IsArithmatic<U>::Value>::Type>
        auto operator/(const U &b) const -> Vector<DIVIDE_T(T, U), length> {
            Vector<DIVIDE_T(T, U), length> result;
            double m = (1.0d / b);
            for (size_t i = 0; i < length; ++i) {
                result[i] = static_cast<DIVIDE_T(T, U)>(this->get(i)) * m;
            }
            return result;
        }

        /**

        @brief Multiplication operator between the vector and a scalar.

        */

        template<typename U, typename = typename jutil::Enable<jutil::Convert<U, T>::Value>::Type>
        auto operator*(const U &n) const -> Vector<MULTIPLY_T(T, U), length> {
            typedef Vector<MULTIPLY_T(T, U), length> rType;
            rType result;
            for (size_t i = 0; i < length; ++i) {
                result[i] = static_cast<MULTIPLY_T(T, U)>(this->get(i)) * static_cast<MULTIPLY_T(T, U)>(n);
            }
            return result;
        }

        /**

        @brief Multiplication operator between two vectors.

        */

        template <typename U>
        auto operator*(const Vector<U, length> &b) const -> MULTIPLY_T(T, U) {
            MULTIPLY_T(T, U) result = static_cast<MULTIPLY_T(T, U)>(0);
            for (size_t i = 0; i < length; ++i) {
                result += static_cast<MULTIPLY_T(T, U)>(get(i)) * static_cast<MULTIPLY_T(T, U)>(b.get(i));
            }
            return result;
        }

        /**

        @brief Assignment operator between two vectors.

        */

        template<typename U>
        auto operator=(const Vector<U, length> &v) -> Vector<T, length> {
            for (size_t i = 0; i < length; ++i) {
                (*this)[i] = static_cast<T>(v.get(i));
            }
            return *this;
        }

        /**

        @brief Assignment operator between the vector and a vector literal.

        */

        auto operator=(Literal v) -> Vector<T, length> {
            rawVector.clear();
            for (size_t i = 0; i < length; ++i) {
                if (i < v.size()) {
                    rawVector.insert(static_cast<T>(*(v.begin() + i)));
                } else if (i >= v.size()) {
                    rawVector.insert(static_cast<T>(0));
                }
            }
            return *this;
        }

        /**

        @brief comparison operator between two vectors.

        */

        template<typename U>
        bool operator==(const Vector<U, length> &v) const {
            for (size_t i = 0; i < length; ++i) {
                if (get(i) != v.get(i)) {
                    return false;
                }
            }
            return true;
        }

        /**

        @brief Negated comparison operator between two vectors.

        */

        template<typename U>
        bool operator!=(const Vector<U, length> &v) const {
            return !(*this == v);
        }

        /**

        @brief Type casting

        */

        template <typename U>
        operator Vector<U, length>() const {
            Vector<U, length> result;
            for (size_t i = 0; i < length; ++i) {
                result[i] = static_cast<U>(this->get(i));
            }
            return result;
        }

        /**

        @brief Multiplication operator between the vector and a matrix.

        */

        template <typename U, size_t rows>
        auto operator*(const Matrix<U, rows, length> &m) const -> Vector<MULTIPLY_T(T, U), rows> {
            Vector<MULTIPLY_T(T, U), rows> result;
            for (size_t i = 0; i < rows; ++i) {
                Vector<U, length> row = m.getRow(i);
                result[i] = row * (*this);
            }
            return result;
        }

        operator const jutil::String() const {
            return asString();
        }

        operator jutil::String() {
            return asString();
        }

        private:
        Raw rawVector;
        jutil::String asString() const {
            jutil::String r = "[";
            for (auto &i: *this) {
                r += jutil::String(i);
                r += ", ";
            }
            r = r.substr(0, -3);
            r += "]";
            return r;
        }
    };

    ///typedefs
    typedef Vector<float, 2> Vector2f;
    typedef Vector<float, 3> Vector3f;
    typedef Vector<float, 4> Vector4f;

    typedef Vector<double, 2> Vector2d;
    typedef Vector<double, 3> Vector3d;
    typedef Vector<double, 4> Vector4d;

    typedef Vector<long double, 2> Vector2ld;
    typedef Vector<long double, 3> Vector3ld;
    typedef Vector<long double, 4> Vector4ld;

    typedef Vector<int, 2> Vector2i;
    typedef Vector<int, 3> Vector3i;
    typedef Vector<int, 4> Vector4i;

    typedef Vector<int8_t, 2> Vector2i8;
    typedef Vector<int8_t, 3> Vector3i8;
    typedef Vector<int8_t, 4> Vector4i8;

    typedef Vector<int16_t, 2> Vector2i16;
    typedef Vector<int16_t, 3> Vector3i16;
    typedef Vector<int16_t, 4> Vector4i16;

    typedef Vector<int32_t, 2> Vector2i32;
    typedef Vector<int32_t, 3> Vector3i32;
    typedef Vector<int32_t, 4> Vector4i32;

    typedef Vector<int64_t, 2> Vector2i64;
    typedef Vector<int64_t, 3> Vector3i64;
    typedef Vector<int64_t, 4> Vector4i64;

    typedef Vector<unsigned, 2> Vector2u;
    typedef Vector<unsigned, 3> Vector3u;
    typedef Vector<unsigned, 4> Vector4u;

    typedef Vector<uint8_t, 2> Vector2u8;
    typedef Vector<uint8_t, 3> Vector3u8;
    typedef Vector<uint8_t, 4> Vector4u8;

    typedef Vector<uint16_t, 2> Vector2u16;
    typedef Vector<uint16_t, 3> Vector3u16;
    typedef Vector<uint16_t, 4> Vector4u16;

    typedef Vector<uint32_t, 2> Vector2u32;
    typedef Vector<uint32_t, 3> Vector3u32;
    typedef Vector<uint32_t, 4> Vector4u32;

    typedef Vector<uint64_t, 2> Vector2u64;
    typedef Vector<uint64_t, 3> Vector3u64;
    typedef Vector<uint64_t, 4> Vector4u64;
}

#endif // VECTOR_H_INCLUDED
