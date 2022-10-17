#pragma once

#include "IIndexer.h"
#include <vector>

class IndexerRowMajor :public IIndexer
{
public:
	IndexerRowMajor(int dimRow, int dimCol);
	int ToFlat(std::initializer_list<int> ijk) const override;
	int GetRowDimension() const;
	int GetColDimension() const;
private:
	int m_dimRow;
	int m_dimCol;
};

