template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T>::Matrix(T value) : _values(Rows * Columns) {
    static_assert(std::is_arithmetic<T>::value, "Can't construct matrix with non integral type");

    for (uint8_t row = 0; row < Rows; ++row) {
        for (uint8_t col = 0; col < Columns; ++col) {
            (*this)(row, col) = value;
        }
    }
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T>::Matrix(const Values& values) : _values{values} {
    LUG_ASSERT(values.size() == Rows * Columns, "Matrix construct with bad size initializer list");
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T>::Matrix(std::initializer_list<T> list) {
    LUG_ASSERT(list.size() == Rows * Columns, "Matrix construct with bad size initializer list");

    auto it = std::begin(list);
    for (uint8_t row = 0; row < Rows; ++row) {
        for (uint8_t col = 0; col < Columns; ++col) {
            (*this)(row, col) = *it;
            it++;
        }
    }
}


template <uint8_t Rows, uint8_t Columns, typename T>
inline constexpr uint8_t Matrix<Rows, Columns, T>::getRows() const {
    return Rows;
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline constexpr uint8_t Matrix<Rows, Columns, T>::getColumns() const {
    return Columns;
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline typename Matrix<Rows, Columns, T>::Values& Matrix<Rows, Columns, T>::getValues() {
    return _values;
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline constexpr const typename Matrix<Rows, Columns, T>::Values& Matrix<Rows, Columns, T>::getValues() const {
    return _values;
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline T& Matrix<Rows, Columns, T>::operator()(uint8_t row, uint8_t col) {
    return _values[col * Rows + row];
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline constexpr const T& Matrix<Rows, Columns, T>::operator()(uint8_t row, uint8_t col) const {
    return _values[col * Rows + row];
}


// Matrix/Scalar operations
template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T>& Matrix<Rows, Columns, T>::operator+=(T rhs) {
    _values += rhs;
    return *this;
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T>& Matrix<Rows, Columns, T>::operator-=(T rhs) {
    _values -= rhs;
    return *this;
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T>& Matrix<Rows, Columns, T>::operator*=(T rhs) {
    _values *= rhs;
    return *this;
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T>& Matrix<Rows, Columns, T>::operator/=(T rhs) {
    _values /= rhs;
    return *this;
}


// Matrix/Matrix operations
template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T>& Matrix<Rows, Columns, T>::operator+=(const Matrix<Rows, Columns, T>& rhs) {
    _values += rhs._values;
    return *this;
}

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T>& Matrix<Rows, Columns, T>::operator-=(const Matrix<Rows, Columns, T>& rhs) {
    _values -= rhs._values;
    return *this;
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename>
Matrix<Rows, Columns, T>& Matrix<Rows, Columns, T>::operator*=(const Matrix<Rows, Columns, T>& rhs)
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == Columns) && EnableBool, Matrix<Rows, Columns, T>>::type& Matrix<Rows, Columns, T>::operator*=(const Matrix<Rows, Columns, T>& rhs)
#endif
{
    static_assert(Rows == Columns, "Operator *= is only available for square matrices");

    *this = *this * rhs;

    return *this;
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename>
Matrix<Rows, Columns, T>& Matrix<Rows, Columns, T>::operator/=(const Matrix<Rows, Columns, T>& rhs)
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == Columns) && EnableBool, Matrix<Rows, Columns, T>>::type& Matrix<Rows, Columns, T>::operator/=(const Matrix<Rows, Columns, T>& rhs)
#endif
{
    static_assert(Rows == Columns, "Operator /= is only available for square matrices");

    *this *= rhs.inverse();

    return *this;
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename>
inline Matrix<Rows, Columns, T> Matrix<Rows, Columns, T>::inverse() const
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == 1) && EnableBool, Matrix<Rows, Columns, T>>::type Matrix<Rows, Columns, T>::inverse() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the inverse");
    return Matrix<Rows, Columns, T>(*this);
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename, typename>
inline Matrix<Rows, Columns, T> Matrix<Rows, Columns, T>::inverse() const
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == 2) && EnableBool, Matrix<Rows, Columns, T>>::type Matrix<Rows, Columns, T>::inverse() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the inverse");
    return (1 / det()) * Matrix<Rows, Columns, T>{
        (*this)(1, 1),
        -(*this)(0, 1),
        -(*this)(1, 0),
        (*this)(0, 0)
    };
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename, typename, typename>
inline Matrix<Rows, Columns, T> Matrix<Rows, Columns, T>::inverse() const
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == 3) && EnableBool, Matrix<Rows, Columns, T>>::type Matrix<Rows, Columns, T>::inverse() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the inverse");
    return (1 / det()) * Matrix<Rows, Columns, T>{
        (*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1),
        (*this)(0, 2) * (*this)(2, 1) - (*this)(0, 1) * (*this)(2, 2),
        (*this)(0, 1) * (*this)(1, 2) - (*this)(0, 2) * (*this)(1, 1),

        (*this)(1, 2) * (*this)(2, 0) - (*this)(1, 0) * (*this)(2, 2),
        (*this)(0, 0) * (*this)(2, 2) - (*this)(0, 2) * (*this)(2, 0),
        (*this)(0, 2) * (*this)(1, 0) - (*this)(0, 0) * (*this)(1, 2),

        (*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0),
        (*this)(0, 1) * (*this)(2, 0) - (*this)(0, 0) * (*this)(2, 1),
        (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0)
    };
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename, typename, typename, typename>
inline Matrix<Rows, Columns, T> Matrix<Rows, Columns, T>::inverse() const
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == 4) && EnableBool, Matrix<Rows, Columns, T>>::type Matrix<Rows, Columns, T>::inverse() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the inverse");
    return (1 / det()) * Matrix<Rows, Columns, T>{
        // 11
          (*this)(1, 1) * (*this)(2, 2) * (*this)(3, 3)
        + (*this)(1, 2) * (*this)(2, 3) * (*this)(3, 1)
        + (*this)(1, 3) * (*this)(2, 1) * (*this)(3, 2)
        - (*this)(1, 1) * (*this)(2, 3) * (*this)(3, 2)
        - (*this)(1, 2) * (*this)(2, 1) * (*this)(3, 3)
        - (*this)(1, 3) * (*this)(2, 2) * (*this)(3, 1),

        // 12
          (*this)(0, 1) * (*this)(2, 3) * (*this)(3, 2)
        + (*this)(0, 2) * (*this)(2, 1) * (*this)(3, 3)
        + (*this)(0, 3) * (*this)(2, 2) * (*this)(3, 1)
        - (*this)(0, 1) * (*this)(2, 2) * (*this)(3, 3)
        - (*this)(0, 2) * (*this)(2, 3) * (*this)(3, 1)
        - (*this)(0, 3) * (*this)(2, 1) * (*this)(3, 2),

        // 13
          (*this)(0, 1) * (*this)(1, 2) * (*this)(3, 3)
        + (*this)(0, 2) * (*this)(1, 3) * (*this)(3, 1)
        + (*this)(0, 3) * (*this)(1, 1) * (*this)(3, 2)
        - (*this)(0, 1) * (*this)(1, 3) * (*this)(3, 2)
        - (*this)(0, 2) * (*this)(1, 1) * (*this)(3, 3)
        - (*this)(0, 3) * (*this)(1, 2) * (*this)(3, 1),

        // 14
          (*this)(0, 1) * (*this)(1, 3) * (*this)(2, 2)
        + (*this)(0, 2) * (*this)(1, 1) * (*this)(2, 3)
        + (*this)(0, 3) * (*this)(1, 2) * (*this)(2, 1)
        - (*this)(0, 1) * (*this)(1, 2) * (*this)(2, 3)
        - (*this)(0, 2) * (*this)(1, 3) * (*this)(2, 1)
        - (*this)(0, 3) * (*this)(1, 1) * (*this)(2, 2),

        // 21
          (*this)(1, 0) * (*this)(2, 3) * (*this)(3, 2)
        + (*this)(1, 2) * (*this)(2, 0) * (*this)(3, 3)
        + (*this)(1, 3) * (*this)(2, 2) * (*this)(3, 0)
        - (*this)(1, 0) * (*this)(2, 2) * (*this)(3, 3)
        - (*this)(1, 2) * (*this)(2, 3) * (*this)(3, 0)
        - (*this)(1, 3) * (*this)(2, 0) * (*this)(3, 2),

        // 22
          (*this)(0, 0) * (*this)(2, 2) * (*this)(3, 3)
        + (*this)(0, 2) * (*this)(2, 3) * (*this)(3, 0)
        + (*this)(0, 3) * (*this)(2, 0) * (*this)(3, 2)
        - (*this)(0, 0) * (*this)(2, 3) * (*this)(3, 2)
        - (*this)(0, 2) * (*this)(2, 0) * (*this)(3, 3)
        - (*this)(0, 3) * (*this)(2, 2) * (*this)(3, 0),

        // 23
          (*this)(0, 0) * (*this)(1, 3) * (*this)(3, 2)
        + (*this)(0, 2) * (*this)(1, 0) * (*this)(3, 3)
        + (*this)(0, 3) * (*this)(1, 2) * (*this)(3, 0)
        - (*this)(0, 0) * (*this)(1, 2) * (*this)(3, 3)
        - (*this)(0, 2) * (*this)(1, 3) * (*this)(3, 0)
        - (*this)(0, 3) * (*this)(1, 0) * (*this)(3, 2),

        // 24
          (*this)(0, 0) * (*this)(1, 2) * (*this)(2, 3)
        + (*this)(0, 2) * (*this)(1, 3) * (*this)(2, 0)
        + (*this)(0, 3) * (*this)(1, 0) * (*this)(2, 2)
        - (*this)(0, 0) * (*this)(1, 3) * (*this)(2, 2)
        - (*this)(0, 2) * (*this)(1, 0) * (*this)(2, 3)
        - (*this)(0, 3) * (*this)(1, 2) * (*this)(2, 0),

        // 31
          (*this)(1, 0) * (*this)(2, 1) * (*this)(3, 3)
        + (*this)(1, 1) * (*this)(2, 3) * (*this)(3, 0)
        + (*this)(1, 3) * (*this)(2, 0) * (*this)(3, 1)
        - (*this)(1, 0) * (*this)(2, 3) * (*this)(3, 1)
        - (*this)(1, 1) * (*this)(2, 0) * (*this)(3, 3)
        - (*this)(1, 3) * (*this)(2, 1) * (*this)(3, 0),

        // 32
          (*this)(0, 0) * (*this)(2, 3) * (*this)(3, 1)
        + (*this)(0, 1) * (*this)(2, 0) * (*this)(3, 3)
        + (*this)(0, 3) * (*this)(2, 1) * (*this)(3, 0)
        - (*this)(0, 0) * (*this)(2, 1) * (*this)(3, 3)
        - (*this)(0, 1) * (*this)(2, 3) * (*this)(3, 0)
        - (*this)(0, 3) * (*this)(2, 0) * (*this)(3, 1),

        // 33
          (*this)(0, 0) * (*this)(1, 1) * (*this)(3, 3)
        + (*this)(0, 1) * (*this)(1, 3) * (*this)(3, 0)
        + (*this)(0, 3) * (*this)(1, 0) * (*this)(3, 1)
        - (*this)(0, 0) * (*this)(1, 3) * (*this)(3, 1)
        - (*this)(0, 1) * (*this)(1, 0) * (*this)(3, 3)
        - (*this)(0, 3) * (*this)(1, 1) * (*this)(3, 0),

        // 34
          (*this)(0, 0) * (*this)(1, 3) * (*this)(2, 1)
        + (*this)(0, 1) * (*this)(1, 0) * (*this)(2, 3)
        + (*this)(0, 3) * (*this)(1, 1) * (*this)(2, 0)
        - (*this)(0, 0) * (*this)(1, 1) * (*this)(2, 3)
        - (*this)(0, 1) * (*this)(1, 3) * (*this)(2, 0)
        - (*this)(0, 3) * (*this)(1, 0) * (*this)(2, 1),

        // 41
          (*this)(1, 0) * (*this)(2, 2) * (*this)(3, 1)
        + (*this)(1, 1) * (*this)(2, 0) * (*this)(3, 2)
        + (*this)(1, 2) * (*this)(2, 1) * (*this)(3, 0)
        - (*this)(1, 0) * (*this)(2, 1) * (*this)(3, 2)
        - (*this)(1, 1) * (*this)(2, 2) * (*this)(3, 0)
        - (*this)(1, 2) * (*this)(2, 0) * (*this)(3, 1),

        // 42
          (*this)(0, 0) * (*this)(2, 1) * (*this)(3, 2)
        + (*this)(0, 1) * (*this)(2, 2) * (*this)(3, 0)
        + (*this)(0, 2) * (*this)(2, 0) * (*this)(3, 1)
        - (*this)(0, 0) * (*this)(2, 2) * (*this)(3, 1)
        - (*this)(0, 1) * (*this)(2, 0) * (*this)(3, 2)
        - (*this)(0, 2) * (*this)(2, 1) * (*this)(3, 0),

        // 43
          (*this)(0, 0) * (*this)(1, 2) * (*this)(3, 1)
        + (*this)(0, 1) * (*this)(1, 0) * (*this)(3, 2)
        + (*this)(0, 2) * (*this)(1, 1) * (*this)(3, 0)
        - (*this)(0, 0) * (*this)(1, 1) * (*this)(3, 2)
        - (*this)(0, 1) * (*this)(1, 2) * (*this)(3, 0)
        - (*this)(0, 2) * (*this)(1, 0) * (*this)(3, 1),

        // 44
          (*this)(0, 0) * (*this)(1, 1) * (*this)(2, 2)
        + (*this)(0, 1) * (*this)(1, 2) * (*this)(2, 0)
        + (*this)(0, 2) * (*this)(1, 0) * (*this)(2, 1)
        - (*this)(0, 0) * (*this)(1, 2) * (*this)(2, 1)
        - (*this)(0, 1) * (*this)(1, 0) * (*this)(2, 2)
        - (*this)(0, 2) * (*this)(1, 1) * (*this)(2, 0)
    };
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Columns, Rows, T> Matrix<Rows, Columns, T>::transpose() const {
    Matrix<Columns, Rows, T> transposeMatrix(0);

    for (uint8_t row = 0; row < Rows; ++row) {
        for (uint8_t col = 0; col < Columns; ++col) {
            transposeMatrix(col, row) = (*this)(row, col);
        }
    }

    return transposeMatrix;
}


template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename>
inline T Matrix<Rows, Columns, T>::det() const
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == 1) && EnableBool, T>::type Matrix<Rows, Columns, T>::det() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the determinant");
    return (*this)(0, 0);
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename, typename>
inline T Matrix<Rows, Columns, T>::det() const
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == 2) && EnableBool, T>::type Matrix<Rows, Columns, T>::det() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the determinant");
    return (
        (*this)(0, 0) * (*this)(1, 1)
        - (*this)(0, 1) * (*this)(1, 0)
    );
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename, typename, typename>
inline T Matrix<Rows, Columns, T>::det() const
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == 3) && EnableBool, T>::type Matrix<Rows, Columns, T>::det() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the determinant");
    return (
        (*this)(0, 0) * (*this)(1, 1) * (*this)(2, 2)
        + (*this)(1, 0) * (*this)(2, 1) * (*this)(0, 2)
        + (*this)(2, 0) * (*this)(0, 1) * (*this)(1, 2)
        - (*this)(0, 0) * (*this)(2, 1) * (*this)(1, 2)
        - (*this)(2, 0) * (*this)(1, 1) * (*this)(0, 2)
        - (*this)(1, 0) * (*this)(0, 1) * (*this)(2, 2)
    );
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename, typename, typename, typename>
inline T Matrix<Rows, Columns, T>::det() const
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == 4) && EnableBool, T>::type Matrix<Rows, Columns, T>::det() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the determinant");
    return (
        (*this)(0, 0) * (*this)(1, 1) * (*this)(2, 2) * (*this)(3, 3)
        + (*this)(0, 0) * (*this)(1, 2) * (*this)(2, 3) * (*this)(3, 1)
        + (*this)(0, 0) * (*this)(1, 3) * (*this)(2, 1) * (*this)(3, 2)

        + (*this)(0, 1) * (*this)(1, 0) * (*this)(2, 3) * (*this)(3, 2)
        + (*this)(0, 1) * (*this)(1, 2) * (*this)(2, 0) * (*this)(3, 3)
        + (*this)(0, 1) * (*this)(1, 3) * (*this)(2, 2) * (*this)(3, 0)

        + (*this)(0, 2) * (*this)(1, 0) * (*this)(2, 1) * (*this)(3, 3)
        + (*this)(0, 2) * (*this)(1, 1) * (*this)(2, 3) * (*this)(3, 0)
        + (*this)(0, 2) * (*this)(1, 3) * (*this)(2, 0) * (*this)(3, 1)

        + (*this)(0, 3) * (*this)(1, 0) * (*this)(2, 2) * (*this)(3, 1)
        + (*this)(0, 3) * (*this)(1, 1) * (*this)(2, 0) * (*this)(3, 2)
        + (*this)(0, 3) * (*this)(1, 2) * (*this)(2, 1) * (*this)(3, 0)

        - (*this)(0, 0) * (*this)(1, 1) * (*this)(2, 3) * (*this)(3, 2)
        - (*this)(0, 0) * (*this)(1, 2) * (*this)(2, 1) * (*this)(3, 3)
        - (*this)(0, 0) * (*this)(1, 3) * (*this)(2, 2) * (*this)(3, 1)

        - (*this)(0, 1) * (*this)(1, 0) * (*this)(2, 2) * (*this)(3, 3)
        - (*this)(0, 1) * (*this)(1, 2) * (*this)(2, 3) * (*this)(3, 0)
        - (*this)(0, 1) * (*this)(1, 3) * (*this)(2, 0) * (*this)(3, 2)

        - (*this)(0, 2) * (*this)(1, 0) * (*this)(2, 3) * (*this)(3, 1)
        - (*this)(0, 2) * (*this)(1, 1) * (*this)(2, 0) * (*this)(3, 3)
        - (*this)(0, 2) * (*this)(1, 3) * (*this)(2, 1) * (*this)(3, 0)

        - (*this)(0, 3) * (*this)(1, 0) * (*this)(2, 1) * (*this)(3, 2)
        - (*this)(0, 3) * (*this)(1, 1) * (*this)(2, 2) * (*this)(3, 0)
        - (*this)(0, 3) * (*this)(1, 2) * (*this)(2, 0) * (*this)(3, 1)
    );
}

template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename, typename, typename, typename, typename>
T Matrix<Rows, Columns, T>::det() const
#else
template <bool EnableBool>
typename std::enable_if<(Rows > 4) && EnableBool, T>::type Matrix<Rows, Columns, T>::det() const
#endif
{
    static_assert(Rows == Columns, "The matrix has to be a square matrix to calculate the determinant");

    T determinant = 0;
    Matrix<Rows - 1, Columns - 1, T> minorMatrix(0);

    constexpr uint8_t k = 0;
    for (uint8_t i = 0; i < Rows; ++i) {
        for (uint8_t row = 0; row < Rows; ++row) {
            if (row == k) {
                continue;
            }

            const uint8_t indexRow = row - (row > k ? 1 : 0);
            for (uint8_t column = 0; column < Columns; ++column) {
                if (column == i) {
                    continue;
                }

                const uint8_t indexColumn = column - (column > i ? 1 : 0);
                minorMatrix(indexRow, indexColumn) = (*this)(row, column);
            }
        }

        determinant += (*this)(i, k) * ((i + k) % 2 ? 1 : -1) * minorMatrix.det();
    }

    return determinant;
}


template <uint8_t Rows, uint8_t Columns, typename T>
#if defined(LUG_COMPILER_MSVC)
template <typename>
inline Matrix<Rows, Columns, T> Matrix<Rows, Columns, T>::identity()
#else
template <bool EnableBool>
inline typename std::enable_if<(Rows == Columns) && EnableBool, Matrix<Rows, Columns, T>>::type Matrix<Rows, Columns, T>::identity()
#endif
{
    static_assert(Rows == Columns, "The identity matrix has to be a square matrix");

    Matrix<Rows, Columns, T> matrix(0);

    for (uint8_t i = 0; i < Rows; ++i) {
        matrix(i, i) = 1;
    }

    return matrix;
}

// Unary operations
template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& lhs) {
    return T(0) - lhs;
}

// Matrix/Scalar operations
template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator+(const Matrix<Rows, Columns, T>& lhs, T rhs) {
    return Matrix<Rows, Columns, T>(lhs.getValues() + rhs);
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& lhs, T rhs) {
    return Matrix<Rows, Columns, T>(lhs.getValues() - rhs);
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator*(const Matrix<Rows, Columns, T>& lhs, T rhs) {
    return Matrix<Rows, Columns, T>(lhs.getValues() * rhs);
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline  Matrix<Rows, Columns, T> operator/(const Matrix<Rows, Columns, T>& lhs, T rhs) {
    return Matrix<Rows, Columns, T>(lhs.getValues() / rhs);
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator+(T lhs, const Matrix<Rows, Columns, T>& rhs) {
    return Matrix<Rows, Columns, T>(lhs + rhs.getValues());
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator-(T lhs, const Matrix<Rows, Columns, T>& rhs) {
    return Matrix<Rows, Columns, T>(lhs - rhs.getValues());
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator*(T lhs, const Matrix<Rows, Columns, T>& rhs) {
    return Matrix<Rows, Columns, T>(lhs * rhs.getValues());
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator/(T lhs, const Matrix<Rows, Columns, T>& rhs) {
    return Matrix<Rows, Columns, T>(lhs / rhs.getValues());
}

// Matrix/Matrix operations
template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator+(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs) {
    Matrix<Rows, Columns, T> matrix{lhs};

    matrix += rhs;

    return matrix;
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs) {
    Matrix<Rows, Columns, T> matrix{lhs};

    matrix -= rhs;

    return matrix;
}

template <uint8_t RowsLeft, uint8_t ColumnsLeft, uint8_t RowsRight, uint8_t ColumnsRight, typename T>
inline Matrix<RowsLeft, ColumnsRight, T> operator*(const Matrix<RowsLeft, ColumnsLeft, T>& lhs, const Matrix<RowsRight, ColumnsRight, T>& rhs) {
    static_assert(ColumnsLeft == RowsRight, "Columns of the right operand and Rows of the left operand must be of the same size to multiply matrices");

    Matrix<RowsLeft, ColumnsRight, T> matrix(0);

    for (uint8_t i = 0; i < RowsLeft; ++i) {
        for (uint8_t j = 0; j < ColumnsRight; ++j) {
            for (uint8_t k = 0; k < RowsRight; ++k) {
                matrix(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }

    return matrix;
}

template <uint8_t RowsLeft, uint8_t ColumnsLeft, uint8_t RowsRight, uint8_t ColumnsRight, typename T>
inline Matrix<RowsLeft, ColumnsRight, T> operator/(const Matrix<RowsLeft, ColumnsLeft, T>& lhs, const Matrix<RowsRight, ColumnsRight, T>& rhs) {
    static_assert(RowsLeft == ColumnsLeft, "Matrix division can only happen with square matrix");
    static_assert(RowsRight == ColumnsRight, "Matrix division can only happen with square matrix");
    static_assert(RowsLeft == RowsRight, "Matrix division can only happen with matrices of the same size");

    return lhs * rhs.inverse();
}


// Comparaison operators
template <uint8_t Rows, uint8_t Columns, typename T>
inline bool operator==(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs) {
    return (lhs.getValues() == rhs.getValues());
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline bool operator!=(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs) {
    return (lhs.getValues() != rhs.getValues());
}

// TODO: Handle alignment of values
template <uint8_t Rows, uint8_t Columns, typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<Rows, Columns, T>& matrix) {
    os << "{\n";

    for (uint8_t i = 0; i < Rows; ++i) {
        os << "  ";

        for (uint8_t j = 0; j < Columns; ++j) {
            if (j != 0) {
                os << ", ";
            }

            os << matrix(i, j);
        }

        os << "\n";
    }

    os << "}";

    return os;
}
