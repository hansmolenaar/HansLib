#pragma once

#include "Utilities/IIndexer.h"


class IndexerKelvinRepr1 : public IIndexer<int>
{
public:
	IndexerKelvinRepr1();
	int ToFlat(std::initializer_list<int> ijk) const override;
};
