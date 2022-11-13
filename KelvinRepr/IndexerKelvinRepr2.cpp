#include "IndexerKelvinRepr2.h"

#include "Utilities/Assert.h"

#include <algorithm>

IndexerKelvinRepr2::IndexerKelvinRepr2()
{
	// Empty
}

int IndexerKelvinRepr2::ToFlat(std::initializer_list<int> ijk) const
{
	Utilities::Assert(2 == ijk.size());
	const int row = *ijk.begin();
	const int col = *(ijk.begin() + 1);

	if (row == col)
	{
		Utilities::Assert(row >= 0 && row < 2);
		return row;
	}
	else
	{
		const int minRC = std::min(row, col);
		const int maxRC = std::max(row, col);
		Utilities::Assert(minRC == 0 && maxRC == 1);
		return 2;
	}
	Utilities::Assert(false);
	return -1;
}