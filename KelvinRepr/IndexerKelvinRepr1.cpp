#include "IndexerKelvinRepr1.h"

#include "Utilities/Assert.h"


IndexerKelvinRepr1::IndexerKelvinRepr1() 
{
	// Empty
}

int IndexerKelvinRepr1::ToFlat(std::initializer_list<int> ijk) const
{
	Utilities::Assert(2 == ijk.size());
	const int row = *ijk.begin();
	const int col = *(ijk.begin() + 1);
	Utilities::Assert(row == 0 && col == 0);
	return 0;
}