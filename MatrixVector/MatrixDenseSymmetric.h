#pragma once

#include "IMatrixSymmetric.h"
#include "IndexerSymmetric.h"
#include <span>
#include <vector>

class MatrixDenseSymmetric : public IMatrixSymmetric
{
  public:
    explicit MatrixDenseSymmetric(int dim);

    int GetDimension() const override;
    void timesVector(std::span<const double>, std::span<double>) const override;

    double operator()(int, int) const;
    double &operator()(int, int);
    double get(int, int) const override;
    void set(int, int, double) override;

  private:
    int m_dim;
    IndexerSymmetric<int> m_indexer;
    std::vector<double> m_matrix;
};
