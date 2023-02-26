#pragma once

#include "IIndexer.h"


class IndexerKelvinRepr1 : public IIndexer<int>
{
public:
	IndexerKelvinRepr1();
	int ToFlat(std::initializer_list<int> ijk) const override;
	int numberOfIndices() const override { return 2; }
};
