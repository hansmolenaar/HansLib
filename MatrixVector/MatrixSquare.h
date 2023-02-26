#pragma once

#include <memory>
#include <vector>
#include <span>
#include "IndexerRowMajor.h"
#include "IMatrixSquare.h"


class MatrixSquare : public IMatrixSquare
{
public:
	explicit MatrixSquare(int dim);

	//void Clear();
	bool Solve(std::span<const double> rhs, std::span< double> sol); // Returns succes

	const IIndexer<int>& GetIndexer();

	int GetDimension() const override;

	double operator() (int, int)  const override;
	double& operator() (int, int) override;


private:

	std::vector<double>  m_matrix;
	std::vector<double>  m_inverse;
	std::vector<int>  m_pivot;
	IndexerRowMajor<int> m_indexer;
	int m_dim;
	bool m_dirty;

};
