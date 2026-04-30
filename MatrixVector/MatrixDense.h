#pragma once

#include <memory>
#include <vector>

#include "IMatrixSquare.h"
#include "IndexerRowMajor.h"
#include <Eigen/Dense>

class MatrixDense : public IMatrix
{
  public:
    MatrixDense(int numRows, int numCols);

    int GetRowDimension() const override;
    int GetColDimension() const override;

    double operator()(int, int) const;
    double &operator()(int, int);
    void set(int, int, double) override;
    double get(int, int) const override;

    void timesVector(std::span<const double>, std::span<double>) const override;

  private:
    Eigen::MatrixXd m_matrix;
};
