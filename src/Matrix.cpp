#include "Matrix.h"

#define nullRef(T) *(T*)0

/*namespace Maths {

        ///error stack is an 8-bit bit-flag structure
        uint8_t errorStack;
        std::ostream defaultOut(NULL);
        std::ostream *out = &defaultOut;
        MatrixRow mNullRow;

        std::ostream &operator<<(std::ostream &o, const Matrix &m) {
            o << "Matrix(" << (int16_t)m.rows() << "x" << (int16_t)m.cols() << ") {\n";
            for (uint8_t i = 0; i < m.rows(); ++i) {
                o << "  [";
                for (uint8_t ii = 0; ii < m.cols(); ++ii) {
                    o << m[i][ii];
                    if (ii != m.cols() - 1) o << ", ";
                }
                o << "]\n";
            }
            o << "}\n";
            return o;
        }

        ///initiates as [0]
        MatrixRow::MatrixRow() : data({0}) {}

        ///@param size of row
        ///initiates with "size" copies of 0
        MatrixRow::MatrixRow(MatIndex n) {
            uint8_t i = 0;
            do {
                data.push_back(0);
                ++i;
            } while (i < n);
        }

        ///@param initializer list containing matrix row's values
        MatrixRow::MatrixRow(const MatRowLiteral &v) {
            data = v;
        }


        ///@param index
        ///@return value at position "index"
        const double &MatrixRow::operator[](MatIndex i) const {
            if (this == 0) return mNullRow[0];
            if (i >= data.size()) {
                Matrix::writeError(MATERR_OUT_OF_BOUNDS);
                return mNullRow[0];
            }
            return data.at(i);
        }
        double &MatrixRow::operator[](MatIndex i) {
            if (this == 0) return mNullRow[0];
            if (i >= data.size()) {
                Matrix::writeError(MATERR_OUT_OF_BOUNDS);
                return mNullRow[0];
            }
            return data.at(i);
        }

        ///@return dot product of two rows
        const double MatrixRow::operator*(const MatrixRow &r) {
            return dotProduct(*this, r);
        }

        ///==
        bool MatrixRow::operator==(const MatrixRow &r) {
            if (cols() != r.cols()) return false;
            for (uint8_t i = 0; i < r.cols(); ++i) {
                if ((*this)[i] != r[i]) return false;
            }
            return true;
        }

        //!=
        bool MatrixRow::operator!=(const MatrixRow &r) {
            return !(*this == r);
        }

        bool MatrixRow::set(MatIndex i, double v) {
            if (this == 0) return false;
            if (i >= data.size()) {
                Matrix::writeError(MATERR_OUT_OF_BOUNDS);
                return false;
            } else {
                data.at(i) = v;
                return true;
            }
        }

        MatrixRow::MatrixRow(const MatrixRow &r) : data({0}) {
            resize(r.cols());
            for (uint8_t i = 0; i < cols(); ++i) {
                set(i, r[i]);
            }
        }

        MatIndex MatrixRow::cols() const {
            if (this == 0) return 0;
            return data.size();
        }

        void MatrixRow::resize(MatIndex s) {
            if (s > cols()) data.resize(s, 0);
            else data.resize(s);
        }

        MatrixRow &MatrixRow::operator=(const MatrixRow &r) {
            resize(r.cols());
            for (uint8_t i = 0; i < cols(); ++i) {
                set(i, r[i]);
            }
            return *this;
        }


        ///@param row count
        ///@param column count
        ///all values initiate as 0
        Matrix::Matrix(MatIndex r, MatIndex c) {
            for (uint8_t i = 0; i < r; ++i) {
                data.push_back(MatrixRow(c));
            }
        }

        ///@param initializer list containing matrix's values
        Matrix::Matrix(const MatLiteral &m) {
            for (MatLiteral::iterator it = m.begin(); it != m.end(); ++it) {
                data.push_back(MatrixRow(*it));
            }

        }

        ///@param index
        ///@return row at position "index"
        const MatrixRow &Matrix::operator[](MatIndex i) const {
            if (i >= rows()) {
                writeError(MATERR_OUT_OF_BOUNDS);
                return nullRef(MatrixRow);
            }
            return data.at(i);
        }
        MatrixRow &Matrix::operator[](MatIndex i) {
            if (i >= rows()) {
                writeError(MATERR_OUT_OF_BOUNDS);
                return nullRef(MatrixRow);
            }
            return data.at(i);
        }

        void Matrix::readErrorsTo(std::ostream &o) {
            out = &o;
        }

        ///@return error at top of error stack
        ///deletes error at top of error stack
        uint8_t Matrix::readError() {
            for (uint16_t i = 0x01; i <= 0x80; i *= 2) {
                if (errorStack & i) {
                    errorStack &= ~i;
                    return i;
                }

            }
            return MATERR_OK;
        }

        void Matrix::writeError(uint8_t e) {
            (*out) << static_cast<uint16_t>(e) << '\n';
            errorStack |= e;
        }

        MatrixRow Matrix::getColumn(MatIndex i) const {
            MatrixRow col(rows());
            if (i >= cols()) {
                writeError(MATERR_OUT_OF_BOUNDS);
                return nullRef(MatrixRow);
            }
            uint8_t c = 0;
            for (auto &ii: data) {
                col.set(c, ii[i]);
                ++c;
            }
            return col;
        }

        MatrixRow Matrix::getColumn(MatIndex i) {
            MatrixRow col(rows());
            if (i >= cols()) {
                writeError(MATERR_OUT_OF_BOUNDS);
                return nullRef(MatrixRow);
            }
            uint8_t c = 0;
            for (auto &ii: data) {
                col.set(c, ii[i]);
                ++c;
            }
            return col;
        }

        ///@return "row count"
        MatIndex Matrix::rows() const {
            return data.size();
        }

        ///@return "column count"
        MatIndex Matrix::cols() const {
            return data.front().cols();
        }

        ///@return transpose of this matrix
        Matrix Matrix::transpose() const {
            Matrix r(cols(), rows());
            for (uint8_t i = 0; i < r.rows(); ++i) {
                r[i] = getColumn(i);
            }
            return r;
        }

        bool Matrix::square() const {
            return (rows() == cols());
        }

        ///@return cofactor of matrix
        Matrix Matrix::cofactor() const {
            if (!square()) {
                writeError(MATERR_NON_SQUARE);
                return nullRef(Matrix);
            }
            Matrix m(rows(), cols());
            int8_t s1 = 1, s2 = 1;
            for (uint8_t i = 0; i < rows(); ++i) {
                for (uint8_t ii = 0; ii < cols(); ++ii) {
                    Matrix sub = subMatrix(i, ii);
                    m[i].set(ii, sub.determinant() * s1 * s2);
                    s2 *= -1;
                }
                s1 *= -1;
            }
            return m;
        }

        Matrix Matrix::subMatrix(MatIndex er, MatIndex ec) const {
            Matrix m(rows() - 1, cols() - 1);
            int8_t r = -1;
            for (uint8_t i = 0; i < rows(); ++i) {
                if (i == er) continue;
                ++r;
                int8_t c = -1;
                for (uint8_t ii = 0; ii < cols(); ++ii) {
                    if (ii == ec) continue;
                    m[r].set(++c, data.at(i)[ii]);
                }
            }
            return m;
        }

        double Matrix::determinant() const {
            if (!square()) {
                writeError(MATERR_NON_SQUARE);
                return nullRef(double);
            }
            if (data.size() == 1) {
                return (*this)[0][0];
            } else if (data.size() == 2) {
                double d = ((*this)[0][0] * (*this)[1][1]) - ((*this)[0][1] * (*this)[1][0]);
                return d;
            } else {
                double r = 0;
                int8_t s = 1;
                for (uint8_t i = 0; i < cols(); ++i) {
                    Matrix sub = subMatrix(0, i);
                    r += s * (*this)[0][i] * sub.determinant();
                    s *= -1;
                }
                return r;
            }
        }


        ///@return inverse of matrix
        Matrix Matrix::inverse() const {
            Matrix c = cofactor(), t = c.transpose();
            return t * (1 / determinant());
        }

        double MatrixRow::dotProduct(const MatrixRow &a, const MatrixRow &b) {
            std::vector<double> r;
            for (unsigned i = 0;  i < a.cols(); ++i) {
                r.push_back(a[i] * b[i]);
            }
            double result = 0;
            for (auto &i: r) {
                result += i;
            }
            return result;
        }

        ///@param size
        ///@return identity matrix with "size" number of rows & columns
        Matrix Matrix::identity(MatIndex s) {
            Matrix r(s, s);
            int i1 = 0;
            for (uint8_t i = 0; i < s; ++i) {
                for (uint8_t ii = 0; ii < s; ++ii) {
                    r[i].set(ii, (ii == i1? 1 : 0));
                }
                ++i1;
            }
            return r;
        }

        /**operator overloads
        -----------------------------------------------------------------------------------------------------------------------------*
        ///+
        const Matrix Matrix::operator+(const Matrix &m) {
            Matrix copy(*this);
            if (copy.rows() != m.rows() || copy.cols() != m.cols()) {
                writeError(MATERR_INAVLID_OPERATION);
                return *this;
            }
            for (uint8_t i = 0; i < copy.rows(); ++i) {
                for (uint8_t ii = 0; ii < copy.cols(); ++ii) {
                    copy[i].set(ii, copy[i][ii] + m[i][ii]);
                }
            }
            return copy;
        }

        ///-
        const Matrix Matrix::operator-(const Matrix &m) {
            Matrix copy(*this);
            if (copy.rows() != m.rows() || copy.cols() != m.cols()) {
                writeError(MATERR_INAVLID_OPERATION);
                return *this;
            }
            for (uint8_t i = 0; i < copy.rows(); ++i) {
                for (uint8_t ii = 0; ii < copy.cols(); ++ii) {
                    copy[i].set(ii, copy[i][ii] - m[i][ii]);
                }
            }
            return copy;
        }

        ///*
        const Matrix Matrix::operator*(const Matrix &m) {
            Matrix result(this->rows(), m.cols());
            if (this->cols() != m.rows()) {
                writeError(MATERR_INAVLID_OPERATION);
            } else {
                for (uint8_t i = 0; i < result.rows(); ++i) {
                    for (uint8_t ii = 0; ii < result.cols(); ++ii) {
                        double d = MatrixRow::dotProduct((*this)[i], m.getColumn(ii));
                        result[i].set(ii, d);
                    }
                }
            }
            return result;
        }
        const Matrix Matrix::operator*(double s) {
            Matrix copy(*this);
            for (uint8_t i = 0; i < copy.rows(); ++i) {
                for (uint8_t ii = 0; ii < copy.cols(); ++ii) {
                    copy[i].set(ii, copy[i][ii] * s);
                }
            }
            return copy;
        }

        ///+=
        Matrix &Matrix::operator+=(const Matrix &m) {
            *this = *this + m;
            return *this;
        }

        ///-=
        Matrix &Matrix::operator-=(const Matrix &m) {
            *this = *this - m;
            return *this;
        }

        ///*=
        Matrix &Matrix::Matrix::operator*=(const Matrix &m) {
            *this = *this * m;
            return *this;
        }
        Matrix &Matrix::operator*=(double s) {
            *this = *this * s;
            return *this;
        }

        ///=
        Matrix &Matrix::operator=(const Matrix &m) {
            data.clear();
            data.resize(m.rows());
            for (uint8_t i = 0; i < m.rows(); ++i) {
                data.push_back(m[i]);
            }
            return *this;
        }

        ///==
        bool Matrix::operator==(const Matrix &m) {
            if (rows() != m.rows() || cols() != m.cols()) return false;
            for (uint8_t i = 0; i < m.rows(); ++i) {
                for (uint8_t ii = 0; ii < m.cols(); ++ii) {
                    if (data.at(i)[ii] != m[i][ii]) return false;
                }
            }
            return true;
        }

        ///!=
        bool Matrix::operator!=(const Matrix &m) {
            return !(*this == m);
        }

        ///---------------------------------------------------------------------------------------------------------------------------
}*/
