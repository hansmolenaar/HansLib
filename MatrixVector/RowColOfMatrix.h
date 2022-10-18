#pragma once

#include "IVector.h"

#include <array>

class IMatrix;

class RowColOfMatrix :public IVector
{
public:
	RowColOfMatrix(IMatrix&, int dim, bool isRow, int startRow, int startCol);
	int GetDimension() const override;

	double operator() (int)  const override;
	double& operator() (int) override;
private:
	IMatrix& m_matrix;
	std::array<int, 2> m_ofset;
	int m_dimension;
	bool m_isRow;
};

