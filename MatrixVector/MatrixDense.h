#pragma once

#include <memory>
#include <vector>

#include "IMatrixSquare.h"
#include "IndexerRowMajor.h"

class MatrixDense : public IMatrix
{
  public:
    MatrixDense(int numRows, int numCols);

    int GetRowDimension() const override;
    int GetColDimension() const override;

    double operator()(int, int) const override;
    double &operator()(int, int) override;

    void timesVector(std::span<const double>, std::span<double>) const;

  private:
    IndexerRowMajor<int> m_indexer;
    std::vector<double> m_entries;
};
