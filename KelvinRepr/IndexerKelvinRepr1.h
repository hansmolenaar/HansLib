#pragma once

#include "HLUtils/IIndexer.h"


class IndexerKelvinRepr1 : public IIndexer
{
public:
	IndexerKelvinRepr1();
	int ToFlat(std::initializer_list<int> ijk) const override;
};
