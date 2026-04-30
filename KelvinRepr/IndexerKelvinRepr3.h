#pragma once

#include "IIndexer.h"

class IndexerKelvinRepr3 : public IIndexer<int>
{
  public:
    IndexerKelvinRepr3();
    size_t toFlat(std::initializer_list<int> ijk) const override;
    int getNumberOfIndices() const override
    {
        return 2;
    }
    size_t getFlatSize() const override
    {
        return NumEntries;
    }
    static void ToRowCol(size_t flat, int &row, int &col);
    static const int NumEntries = 6;
};
