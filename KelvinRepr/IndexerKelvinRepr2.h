#pragma once

#include "HLUtils/IIndexer.h"


class IndexerKelvinRepr2 : public IIndexer<int>
{
public:
	IndexerKelvinRepr2();
	int ToFlat(std::initializer_list<int> ijk) const override;
};

