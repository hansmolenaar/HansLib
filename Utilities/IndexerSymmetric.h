#pragma once

#include "IIndexer.h"
#include <vector>


class IndexerSymmetric :public IIndexer<int>
{
public:
	IndexerSymmetric(int dim);
	size_t ToFlat(std::initializer_list<int> ijk) const override;
	int numberOfIndices() const override;
	size_t ToFlat(int, int) const;

private:
	int m_dim;
};

