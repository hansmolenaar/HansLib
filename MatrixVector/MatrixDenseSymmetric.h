#pragma once

#include "EigenSolution.h"
#include "IMatrixSymmetric.h"
#include "IndexerSymmetric.h"
#include <Eigen/Dense>
#include <span>
#include <vector>

class MatrixDenseSymmetric : public IMatrixSymmetric
{
  public:
    explicit MatrixDenseSymmetric(int dim);

    int GetDimension() const override;
    void timesVector(std::span<const double>, std::span<double>) const override;

    bool Solve(std::span<const double> rhs, std::span<double> sol); // Returns succes
    EigenSolution getEigenSolution();

    double operator()(int, int) const;
    double get(int, int) const override;
    void set(int, int, double) override;

  private:
    Eigen::MatrixXd m_matrix;
};
