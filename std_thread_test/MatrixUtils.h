#ifndef MATRIX_MULTIPLY_H__
#define MATRIX_MULTIPLY_H__
#include <memory>
#include <algorithm>
#include <cstring>
#include <iostream>

class Matrix {
    std::unique_ptr<int[]> _values;
    int _cols;
    int _rows;

public:
    Matrix() : _cols(0), _rows(0), _values(nullptr) {}
    Matrix(int cols, int rows) : _cols(cols), _rows(rows) {
        _values.reset(new int[_cols * _rows]);
        memset(getValues(), 0, _cols * _rows * sizeof(int));
    }
    Matrix(const Matrix& m) : _cols(m._cols), _rows(m._rows) {
        _values.reset(new int[_cols * _rows]);
        std::copy_n(m.getValues(), _cols * _rows, getValues());
    }
    Matrix(Matrix&& m) : _cols(m._cols), _rows(m._rows) {
        _values = std::move(m._values);
    }
    void operator=(const Matrix& m) {
        _cols = m._cols;
        _rows = m._rows;
        _values.reset(new int[_cols * _rows]);
        std::copy_n(m.getValues(), _cols * _rows, getValues());
    }
    void operator=(Matrix&& m) {
        _cols = m._cols;
        _rows = m._rows;
        _values = std::move(m._values);
    }
    bool operator==(const Matrix& m) {
        if(std::memcmp(m.getValues(), getValues(), _cols * _rows) == 0 && _cols == m._cols && _rows == m._rows) {
            return true;
        }

        return false;
    }

    int getCols() const { return _cols; }
    int getRows() const { return _rows; }
    int* getValues() const { return _values.get(); }
    void transpose();
};

class MatrixMultiplier {
    const Matrix& multiplied;
    const Matrix& multiplier;
    Matrix result;
    int fromRow = 0;
    int toRow = 0;
    bool isMultiplierTransposed;


    void multiply(const Matrix& transposed);

public:
    MatrixMultiplier(const Matrix& a, const Matrix& b, bool transposed = false) : 
                    multiplied(a), multiplier(b), isMultiplierTransposed(transposed) {}
    void operator()();
    void setFromRow(int row) { fromRow = row; }
    void setToRow(int row) { toRow = row; }
    Matrix& getResult() { return result; }
};

class MatrixUtils {
public:
    static Matrix multiplyParalel(const Matrix& a, const Matrix& b, int threadsCount);
    static void multiplyParalelThread(const Matrix& a, const Matrix& b, int fromRow, int toRow, Matrix& c);
    static Matrix multiply(const Matrix& a, const Matrix& b);
    static Matrix multiplySlow(const Matrix& a, const Matrix& b);
    static void PrintMatrix(const Matrix& m, std::ostream& outStream = std::cout);
    static Matrix random(int cols, int rows);
};

#endif // MATRIX_MULTIPLY_H__