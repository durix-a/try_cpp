#include "MatrixUtils.h"
#include <stdexcept>
#include <random>
#include <thread>

using namespace std;

void MatrixMultiplier::operator()() {
    if(isMultiplierTransposed) {
        multiply(multiplier);
    } else {
        Matrix transposed = multiplier;

        transposed.transpose();
        multiply(transposed);
    }
}

void MatrixMultiplier::multiply(const Matrix& transposed) {
    result = move(Matrix(transposed.getRows(), multiplied.getRows()));
    int* a_values = multiplied.getValues();
    int* b_values = transposed.getValues();
    int* c_values = result.getValues();
    int t_rows = transposed.getRows();
    int m_cols = multiplied.getCols();
    int t_cols = transposed.getCols();

    if(fromRow == 0 && toRow == 0) {
        toRow = multiplied.getRows();
    }

    for(size_t i = fromRow; i < toRow; i++) {
        int b_col_position = t_rows * i;
        int a_col_position = m_cols * i;

        for(size_t j = 0; j < t_rows; j++) {
            int c_values_position = b_col_position + j;
            int b_row_position = t_cols * j;

            for(size_t l = 0; l < m_cols; l++) {
                c_values[c_values_position] += a_values[a_col_position + l] * b_values[b_row_position + l];
            }
        }
    }
}

Matrix MatrixUtils::multiplyParalel(const Matrix& a, const Matrix& b, int threadsCount) {
    if(a.getCols() != b.getRows() || a.getRows() != b.getCols()) {

        throw std::invalid_argument("wrong matrix dimentions");
    }

    Matrix transpose = b;
    thread* multiplier[threadsCount];
    int rowsPerThread = a.getRows() / threadsCount;

    transpose.transpose();
    MatrixMultiplier mm(a, transpose, true);

    int i = 0;
    for(size_t i = 0; i < (threadsCount - 1); i++)
    {
        mm.setFromRow(rowsPerThread * i);
        mm.setToRow(rowsPerThread * (i + 1));
        multiplier[i] = new thread(ref(mm));
    }

    mm.setFromRow(rowsPerThread * (threadsCount - 1));
    mm.setToRow(a.getRows());
    multiplier[threadsCount - 1] = new thread(ref(mm));
    
    for(size_t i = 0; i < threadsCount; i++)
    {
        multiplier[i]->join();
        delete multiplier[i];
    }

    return move(mm.getResult());
}

Matrix MatrixUtils::multiply(const Matrix& a, const Matrix& b) {
    int a_cols = a.getCols();
    int a_rows = a.getRows();
    int b_cols = b.getCols();
    int b_rows = b.getRows();
    
    if(a_cols != b_rows || a_rows != b_cols) {

        throw std::invalid_argument("wrong matrix dimentions");
    }

    Matrix c(b_cols, a_rows);
    int* a_values = a.getValues();
    int* c_values = c.getValues();

    Matrix transpose = b;

    transpose.transpose();
    int* b_transpose_values = transpose.getValues();

    for(size_t i = 0; i < a_rows; i++) {
        int b_col_position = b_cols * i;
        int a_col_position = a_cols * i;

        for(size_t j = 0; j < b_cols; j++) {
            int c_values_position = b_col_position + j;
            int b_row_position = b_rows * j;

            for(size_t l = 0; l < a_cols; l++) {
                c_values[c_values_position] += a_values[a_col_position + l] * b_transpose_values[b_row_position + l];
            }
        }
    }
    
    return c;
}

Matrix MatrixUtils::multiplySlow(const Matrix& a, const Matrix& b) {
    int a_cols = a.getCols();
    int a_rows = a.getRows();
    int b_cols = b.getCols();
    int b_rows = b.getRows();
    
    if(a_cols != b_rows || a_rows != b_cols) {

        throw std::invalid_argument("wrong matrix dimentions");
    }

    Matrix c(b_cols, a_rows);
    int* a_values = a.getValues();
    int* b_values = b.getValues();
    int* c_values = c.getValues();

    for(size_t i = 0; i < a_rows; i++) {
        int b_col_position = b_cols * i;
        int a_col_position = a_cols * i;

        for(size_t j = 0; j < b_cols; j++) {
            int c_values_position = b_col_position + j;

            for(size_t l = 0; l < a_cols; l++) {
                c_values[c_values_position] += a_values[a_col_position + l] * b_values[b_cols * l + j];
            }
        }
    }
    
    return c;
}

void MatrixUtils::PrintMatrix(const Matrix& m, ostream& outStream) {
    int m_cols = m.getCols();
    int m_rows = m.getRows();
    int* m_values = m.getValues();

    for(size_t j = 0; j < m_rows; j++) {
        for(size_t i = 0; i < m_cols; i++) {
            outStream << m_values[j * m_cols + i] << " ";
        }

        outStream << endl;
    }
}

Matrix MatrixUtils::random(int cols, int rows) {
    Matrix m(cols, rows);
    int* m_values = m.getValues();
    random_device r;
    default_random_engine e1(r());
    uniform_int_distribution<int> uniform_dist(0, 9);

    for(size_t j = 0; j < rows; j++) {
        for(size_t i = 0; i < cols; i++) {
            m_values[j * cols + i] = uniform_dist(e1);
        }
    }

    return m;
}

void Matrix::transpose() {
    int* transpose_values = new int[_cols * _rows];
    int* values = getValues();

    for(size_t i = 0; i < _rows; i++) {
        for(size_t j = 0; j < _cols; j++) {
            transpose_values[_rows * j + i] = values[_cols * i + j];
        }
    }

    _values.reset(transpose_values);
    swap(_cols, _rows);
}