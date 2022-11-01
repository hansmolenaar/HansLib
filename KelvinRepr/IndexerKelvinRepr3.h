#pragma once

#include "Utilities/IIndexer.h"


class IndexerKelvinRepr3 : public IIndexer<int>
{
public:
   IndexerKelvinRepr3();
   int ToFlat(std::initializer_list<int> ijk) const override;
   int numberOfIndices() const override { return 2; }
   static void ToRowCol(int flat, int&row, int&col);
   static const int NumEntries = 6;
};

