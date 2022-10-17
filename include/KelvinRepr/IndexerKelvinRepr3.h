#pragma once

#include "Utils/IIndexer.h"


class IndexerKelvinRepr3 : public IIndexer
{
public:
   IndexerKelvinRepr3();
   int ToFlat(std::initializer_list<int> ijk) const override;
   static void ToRowCol(int flat, int&row, int&col);
   static const int NumEntries = 6;
};

