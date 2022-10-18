#pragma once

#include "IIndexer.h"
#include <vector>

class IndexerSymmetric :public IIndexer
{
public:
	IndexerSymmetric(int dim);
	int ToFlat(std::initializer_list<int> ijk) const override;
	int ToFlat(int, int) const;
private:
	int m_dim;
};

