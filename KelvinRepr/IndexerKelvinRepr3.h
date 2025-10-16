#pragma once

#include "IIndexer.h"


class IndexerKelvinRepr3 : public IIndexer<int>
{
public:
   IndexerKelvinRepr3();
   size_t ToFlat(std::initializer_list<int> ijk) const override;
   int numberOfIndices() const override { return 2; }
   static void ToRowCol(size_t flat, int&row, int&col);
   static const int NumEntries = 6;
};

