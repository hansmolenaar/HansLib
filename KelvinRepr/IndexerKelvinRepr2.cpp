#include "IndexerKelvinRepr2.h"

#include "MyAssert.h"

#include <algorithm>

IndexerKelvinRepr2::IndexerKelvinRepr2()
{
	// Empty
}

size_t IndexerKelvinRepr2::ToFlat(std::initializer_list<int> ijk) const
{
	Utilities::MyAssert(2 == ijk.size());
	const int row = *ijk.begin();
	const int col = *(ijk.begin() + 1);

	if (row == col)
	{
		Utilities::MyAssert(row >= 0 && row < 2);
		return row;
	}
	else
	{
		const int minRC = std::min(row, col);
		const int maxRC = std::max(row, col);
		Utilities::MyAssert(minRC == 0 && maxRC == 1);
		return 2;
	}
	Utilities::MyAssert(false);
	return -1;
}