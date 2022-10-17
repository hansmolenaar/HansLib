#pragma once

#include "Utils/IIndexer.h"


class IndexerKelvinRepr1 : public IIndexer
{
public:
	IndexerKelvinRepr1();
	int ToFlat(std::initializer_list<int> ijk) const override;
};
