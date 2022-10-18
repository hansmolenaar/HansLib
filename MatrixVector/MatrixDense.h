#pragma once

#include <memory>
#include <vector>

#include "HLUtils/IndexerRowMajor.h"
#include "IMatrixSquare.h"

class IIndexer;

class MatrixDense : public IMatrix
{
public:
	MatrixDense(int numRows, int numCols);


	int GetRowDimension() const override;
	int GetColDimension() const override;

	double operator() (int, int)  const override;
	double& operator() (int, int) override;


private:
	IndexerRowMajor m_indexer;
	std::vector<double>  m_entries;
};
