#pragma once

#include "Utils/IIndexer.h"


class IndexerKelvinRepr2 : public IIndexer
{
public:
	IndexerKelvinRepr2();
	int ToFlat(std::initializer_list<int> ijk) const override;
};

