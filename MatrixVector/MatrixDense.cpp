#include "MatrixDense.h"
#include "IMatrixUtils.h"
#include "Iota.h"

namespace
{
using EigenColumnVector = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using EigenMapVectorType = Eigen::Map<EigenColumnVector>;
using EigenMapVectorTypeConst = Eigen::Map<const EigenColumnVector>;
} // namespace

MatrixDense::MatrixDense(int numRows, int numCols) : m_matrix(numRows, numCols)
{
    m_matrix.setZero();
}

int MatrixDense::GetRowDimension() const
{
    return m_matrix.rows();
}

int MatrixDense::GetColDimension() const
{
    return m_matrix.cols();
}

double MatrixDense::operator()(int row, int col) const
{
    return m_matrix(row, col);
}

double &MatrixDense::operator()(int row, int col)
{
    return m_matrix(row, col);
}

double MatrixDense::get(int row, int col) const
{
    return m_matrix(row, col);
}

void MatrixDense::set(int row, int col, double value)
{
    m_matrix(row, col) = value;
}

void MatrixDense::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    EigenMapVectorType resultMapped(result.data(), result.size());
    EigenMapVectorTypeConst vecinMapped(vecin.data(), vecin.size());

    resultMapped = m_matrix * vecinMapped;
}
