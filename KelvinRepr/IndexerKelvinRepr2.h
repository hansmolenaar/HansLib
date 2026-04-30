#pragma once

#include "IIndexer.h"

class IndexerKelvinRepr2 : public IIndexer<int>
{
  public:
    IndexerKelvinRepr2();
    size_t toFlat(std::initializer_list<int> ijk) const override;
    int getNumberOfIndices() const override
    {
        return 2;
    }
    size_t getFlatSize() const override
    {
        return 3;
    }
};
