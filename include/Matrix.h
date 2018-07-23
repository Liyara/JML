#ifndef JML_MATRIX_H
#define JML_MATRIX_H

/**

@file       Matrix.hpp
@author     Jarrett W. Kastler (Liyara)
@date       12/1/2016
@version    2.0

@section    DESCRIPTION
Implements a Matrix class designed to mimic the behaviour of mathematical Matrices.

*/

#include "Vector.hpp"
#include <IO.h>
#include "trig.h"

///error codes
#define MATERR                          0x02
#define MATERR_OK                       0x00 ///@e No error
#define MATERR_INAVLID_OPERATION        0x01 ///@e arithmetic operation failed
#define MATERR_OUT_OF_BOUNDS            0x02 ///@e Got index not within bounds of matrix
#define MATERR_NON_SQUARE               0x04 ///@e Non-square matrix used in square-dependent operation
#define MATERR_INCOMPLETE               0x08 ///@e Initializer list provided for matrix initialization does not form complete rectangle

namespace jml {

    /**

    @param T: Base numerical type to be stored in the matrix.
    @param rows: Number of rows in the matrix.
    @param cols: Numbers of columns in the matrix.
    @param Enforces a minumum number of rows of 2.
    @param Enforces a minimum number of rows of 2.

    */
    template<
        typename T,
        size_t rows,
        size_t cols,
        typename
    >
    class Matrix : public jutil::StringInterface {

    public:

        static constexpr size_t nRows = rows;
        static constexpr size_t nCols = cols;

        typedef Matrix<T, rows, cols> Type;
        typedef T ValueType;

        typedef std::initializer_list<std::initializer_list<ValueType> > Literal;
        typedef Vector<ValueType, cols> *Iterator;
        typedef const Vector<ValueType, cols>* ConstIterator;

        void array(T arr[rows * cols]) {
            size_t c = 0;
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    arr[c] = (*this)[i][j];
                    ++c;
                }
            }
        }

        /**

        @brief Constructor which fills all of the spaces in the matrix with 0.

        */

        Matrix() {
            populateRows(static_cast<ValueType>(0));
        }

        /**

        @brief Constructor which fills all of the spaces in the matrix with @param n.

        */

        Matrix(const ValueType &n) {
            populateRows(n);
        }

        /**

        @brief Constructor which fills each space of the matrix with the corresponding value in @param m.

        */

        Matrix(Literal m) {
            data.reserve(rows);
            for (size_t i = 0; i < rows; ++i) {
                if (i < m.size()) {
                    data.insert(Vector<T, cols>(*(m.begin() + i)));
                } else {
                    data.insert(Vector<T, cols>(static_cast<T>(0)));
                }
            }
        }

        /**

        @return Value in matrix at position ( @param x, @param y ).
        @param x: Row index to search.
        @param y: Column index to search.

        */

        T get(size_t x, size_t y) const {
            return getRow(x).get(y);
        }

        /**

        @return Vector containing all of the values in matrix's row @param i.
        @param i: Row index.

        */

        Vector<T, cols> getRow(size_t i) const {
            return data[i];
        }

        /**

        @return Vector containing all of the values in matrix's column @param i.
        @param i: Column index.

        */

        Vector<T, rows> getCol(size_t i) const {
            Vector<T, rows> result;
            for (size_t j = 0; j < rows; ++j) {
                result[static_cast<size_t>(j)] = getRow(j).get(i);
            }
            return result;
        }

        /**

        @return Reference to a vector containing all of the values in matrix's row @param i.
        @param i: Row index.

        */

        Vector<T, cols> &operator[](size_t i) {
            return data[i];
        }

        /**

        @return Number of ros in the matrix.

        */

        constexpr size_t numRows() const {
            return rows;
        }

        /**

        @return Number of columns in the matrix.

        */

        constexpr size_t numCols() const {
            return cols;
        }

        /**

        @return Pointer to the first row of the matrix.

        */

        Iterator begin() {
            return &((*this)[0]);
        }

        /**

        @return Const pointer to the first row of the matrix.

        */

        ConstIterator begin() const {
            return &((*this)[0]);
        }

        /**

        @return Pointer to the last row of the matrix.

        */

        Iterator end() {
            return (&((*this)[rows - 1]) + 1);
        }

        /**

        @return Const pointer to the last row of the matrix.

        */

        ConstIterator end() const {
            return (&((*this)[rows - 1]) + 1);
        }

        /**

        @return [true]: The matrix is square.
        @return [false]: The matrix is not square.

        */

        inline constexpr bool square() const {
            return cols == rows;
        }

        /**

        @return Matrix Identical to the current matrix excluding row @param er and column @param ec.
        @param fRows: Number of rows to be in the resulting matrix. Should always be passed as 1 less than the number of rows in the matrix.
        @param fCols: Number of columns to be in the resulting matrix. Should always be passed as 1 less than the number of columns in the matrix.
        @param er: Row index to be excluded from matrix.
        @param ec: Column index to be excluded from matrix.
        @warning Only usable with with matrices with row and column count of 3 or above.

        */

        template <size_t fRows, size_t fCols>
        Matrix<T, fRows, fCols> subMatrix(size_t er, size_t ec) const {
            Matrix<T, fRows, fCols> result;
            int8_t r = -1;
            for (size_t i = 0; i < rows; ++i) {
                if (i == er) continue;
                ++r;
                int8_t c = -1;
                for (size_t j = 0; j < cols; ++j) {
                    if (j == ec) continue;
                    result[r][++c] = get(i, j);
                }
            }
            return result;
        }

        /**

        @return Determinant of the matrix.
        @warning Only usable with with square matrices.

        */

        long double determinant() const {
            static_assert(rows == cols, "Attempting to calculate determinant of nonsquare matrix.");
            JUTIL_IFCX_(rows == 2) {
                return ((static_cast<long double>(this->get(0, 0)) * static_cast<long double>(this->get(1, 1))) - (static_cast<long double>(this->get(0, 1)) * static_cast<long double>(this->get(1, 0))));
            } else {
                long double r = 0;
                int8_t s = 1;
                for (size_t i = 0; i < cols; ++i) {
                    constexpr size_t rr = (rows <= 2? rows : rows - 1);
                    constexpr size_t cc = (cols <= 2? cols : cols - 1);
                    Matrix<T, rr, cc> sub = subMatrix<rr, cc>(0, i);
                    r += static_cast<long double>(s) * static_cast<long double>(this->get(0, i)) * sub.determinant();
                    s *= -1;
                }
                return r;
            }
        }

        /**

        @return Cofactor of the matrix.
        @warning Only usable with with square matrices.

        */

        Matrix<long double, rows, cols> cofactor() const {
            static_assert(rows == cols, "Attempting to calculate cofactor of nonsquare matrix.");
            JUTIL_IFCX_(rows == 2 && cols == 2) {
                Matrix<long double, rows, cols> result({
                    {static_cast<long double>(get(1, 1)), static_cast<long double>(get(1, 0) * -1.0L)},
                    {static_cast<long double>(get(0, 1) * -1.0L), static_cast<long double>(get(0, 0))}
                });
                return result;
            } else {
                Matrix<long double, rows, cols> m;
                int8_t s = 1;
                for (size_t i = 0; i < rows; ++i) {
                    for (size_t j = 0; j < cols; ++j) {
                        constexpr size_t rr = (rows <= 2? rows : rows - 1);
                        constexpr size_t cc = (cols <= 2? cols : cols - 1);
                        Matrix<T, rr, cc> sub = subMatrix<rr, cc>(i, j);
                        m[i][j] = sub.determinant() * static_cast<long double>(s);
                        s *= -1;
                    }
                }
                return m;
            }
        }

        /**

        @return Transpose of the matrix.

        */

        Matrix<T, cols, rows> transpose() const {
            Matrix<T, cols, rows> result;
            for (size_t i = 0; i < cols; ++i) {
                result[i] = getCol(i);
            }
            return result;
        }

        /**

        @return Adjugate of the matrix.
        @warning Only usable with with square matrices.

        */

        Matrix<long double, rows, cols> adjugate() const {
            static_assert(rows == cols, "Attempting to calculate adjugate of nonsquare matrix.");
            JUTIL_IFCX_(rows == 2 && cols == 2) {
                Matrix<long double, rows, cols> result({
                    {static_cast<long double>(get(1, 1)), static_cast<long double>(get(0, 1) * -1.0L)},
                    {static_cast<long double>(get(1, 0) * -1.0L), static_cast<long double>(get(0, 0))}
                });
                return result;
            } else {
                return cofactor().transpose();
            }
        }

        /**

        @return Inverse of the matrix.
        @warning Only usable with with square matrices.

        */

        Matrix<long double, rows, cols> inverse() const {
            static_assert(rows == cols, "Attempting to invert nonsquare matrix.");
            return adjugate() * (1.0L / determinant());
        }

        /**

        @brief Addition operator between two matrices.

        */

        template <typename U>
        auto operator+(const Matrix<U, rows, cols> &b) const -> Matrix<ADD_T(T, U), rows, cols> {
            Matrix<ADD_T(T, U), rows, cols> result;
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[i][j] = static_cast<ADD_T(T, U)>(this->get(i, j)) + static_cast<ADD_T(T, U)>(b.get(i, j));
                }
            }
            return result;
        }

        /**

        @brief Subtraction operator between two matrices.

        */

        template <typename U>
        auto operator-(const Matrix<U, rows, cols> &b) const -> Matrix<SUBTRACT_T(T, U), rows, cols> {
            return *this + (b * -1);
        }

        /**

        @brief Multiplication operator matrix and scalar.

        */

        template <typename U>
        auto operator*(const U &n) const -> Matrix<MULTIPLY_T(T, U), rows, cols> {
            Matrix<MULTIPLY_T(T, U), rows, cols> result;
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[i][j] = static_cast<MULTIPLY_T(T, U)>(this->get(i, j)) * static_cast<MULTIPLY_T(T, U)>(n);
                }
            }
            return result;
        }

        /**

        @brief Multiplication operator between two matrices.

        */

        template <typename U, size_t bCols>
        auto operator*(const Matrix<U, cols, bCols> &b) const -> Matrix<MULTIPLY_T(T, U), rows, bCols> {
            Matrix<MULTIPLY_T(T, U), rows, bCols> result;
            for (size_t i = 0; i < rows; ++i) {
                Vector<T, cols> lVec = this->getRow(i);
                Vector<MULTIPLY_T(T, U), bCols> resultVec;
                for (size_t j = 0; j < b.numCols(); ++j) {
                    resultVec[j] = lVec * b.getCol(j);
                }
                result[i] = resultVec;
            }
            return result;
        }

        template <typename U>
        auto operator()(const Matrix<U, rows, cols> &b) const -> Matrix<MULTIPLY_T(T, U), rows, cols> {
            Matrix<MULTIPLY_T(T, U), rows, cols> result = (*this);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[i][j] *= b.getRow(i)[j];
                }
            }
            return result;
        }

        /**

        @brief Multiplication operator between matrix and vector.

        */

        template <typename U>
        auto operator*(const Vector<U, cols> &v) const -> Vector<MULTIPLY_T(T, U), rows> {
            Vector<MULTIPLY_T(T, U), rows> result;
            for (size_t i = 0; i < rows; ++i) {
                Vector<U, cols> row = getRow(i);
                result[i] = row * v;
            }
            return result;
        }

        /**

        @brief Division operator between two matrices.

        */

        template <typename U>
        auto operator/(const Matrix<U, rows, cols> &m) const -> Matrix<long double, rows, cols> {
            static_assert(rows == cols, "Attempting to divide nonsquare matrices.");
            Matrix<long double, rows, cols> result = *this;
            result = (*this) * m.inverse();
            return result;
        }

        /**

        @brief Assignment operator between two matrices.

        */

        template <typename U>
        auto operator=(const Matrix<U, rows, cols> &b) -> Matrix<T, rows, cols>& {
            for (size_t i = 0; i < rows; ++i) {
                (*this)[i] = static_cast<Vector<T, cols> >(b.getRow(i));
            }
            return *this;
        }

        /**

        @brief  Casts to matrix of @param U type.

        */

        template <typename U>
        operator Matrix<U, rows, cols>() const {
            Matrix<U, rows, cols> result;
            for (size_t i = 0; i < rows; ++i) {
                result[i] = static_cast<Vector<U, cols> >(this->getRow(i));
            }
            return result;
        }

        explicit operator jutil::String() {
            return asString();
        }

        explicit operator const jutil::String() const {
            return asString();
        }



    private:
        jutil::Queue<Vector<T, cols> > data;

        void populateRows(const T &n) {
            data.clear();
            data.reserve(rows);
            for (size_t i = 0; i < rows; ++i) {
                data.insert(Vector<T, cols>(n));
            }
        }

        jutil::String asString() const {
            jutil::String r;
            for (auto &i: data) {
                for (auto &ii: i) {
                    r += jutil::String(ii) + jutil::String('\t');
                }
                r += '\n';
            }
            return r;
        }
    };

    typedef Matrix<float, 2, 2> Matrix2f;
    typedef Matrix<float, 3, 3> Matrix3f;
    typedef Matrix<float, 4, 4> Matrix4f;

    typedef Matrix<double, 2, 2> Matrix2d;
    typedef Matrix<double, 3, 3> Matrix3d;
    typedef Matrix<double, 4, 4> Matrix4d;

    typedef Matrix<int, 2, 2> Matrix2i;
    typedef Matrix<int, 3, 3> Matrix3i;
    typedef Matrix<int, 4, 4> Matrix4i;

    typedef Matrix<int8_t, 2, 2> Matrix2i8;
    typedef Matrix<int8_t, 3, 3> Matrix3i8;
    typedef Matrix<int8_t, 4, 4> Matrix4i8;

    typedef Matrix<int16_t, 2, 2> Matrix2i16;
    typedef Matrix<int16_t, 3, 3> Matrix3i16;
    typedef Matrix<int16_t, 4, 4> Matrix4i16;

    typedef Matrix<int32_t, 2, 2> Matrix2i32;
    typedef Matrix<int32_t, 3, 3> Matrix3i32;
    typedef Matrix<int32_t, 4, 4> Matrix4i32;

    typedef Matrix<int64_t, 2, 2> Matrix2i64;
    typedef Matrix<int64_t, 3, 3> Matrix3i64;
    typedef Matrix<int64_t, 4, 4> Matrix4i64;

    typedef Matrix<unsigned, 2, 2> Matrix2u;
    typedef Matrix<unsigned, 3, 3> Matrix3u;
    typedef Matrix<unsigned, 4, 4> Matrix4u;

    typedef Matrix<uint8_t, 2, 2> Matrix2u8;
    typedef Matrix<uint8_t, 3, 3> Matrix3u8;
    typedef Matrix<uint8_t, 4, 4> Matrix4u8;

    typedef Matrix<uint16_t, 2, 2> Matrix2u16;
    typedef Matrix<uint16_t, 3, 3> Matrix3u16;
    typedef Matrix<uint16_t, 4, 4> Matrix4u16;

    typedef Matrix<uint32_t, 2, 2> Matrix2u32;
    typedef Matrix<uint32_t, 3, 3> Matrix3u32;
    typedef Matrix<uint32_t, 4, 4> Matrix4u32;

    typedef Matrix<uint64_t, 2, 2> Matrix2u64;
    typedef Matrix<uint64_t, 3, 3> Matrix3u64;
    typedef Matrix<uint64_t, 4, 4> Matrix4u64;

}

#define JML_MATRIX_APPLY(m, f, ...) for (auto &__jml_ele: m) { \
    for (auto &jml_ele: __jml_ele) {\
        jml_ele = f(__VA_ARGS__);\
    }\
}

#endif // JML_MATRIX_H
