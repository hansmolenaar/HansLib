#pragma once

#include "IMatrixSquare.h"
#include "IndexerRowMajor.h"
#include <Eigen/Dense>
#include <memory>
#include <span>
#include <vector>

class MatrixSquare : public IMatrixSquare
{
  public:
    explicit MatrixSquare(int dim);

    bool Solve(std::span<const double> rhs, std::span<double> sol); // Returns succes

    int GetDimension() const override;
    void timesVector(std::span<const double>, std::span<double>) const override;

    double operator()(int, int) const;
    double get(int, int) const override;
    void set(int, int, double) override;
    double &operator()(int, int);

  private:
    Eigen::MatrixXd m_matrix;
};
