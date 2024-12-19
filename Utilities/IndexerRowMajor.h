#pragma once

#include "IIndexer.h"
#include "MyAssert.h"
#include <vector>

template <typename I>
class IndexerRowMajor :public IIndexer<I>
{
public:
	IndexerRowMajor(I dimRow, I dimCol);
	size_t ToFlat(std::initializer_list<I> ijk) const override;
	I numberOfIndices() const override;
	I GetRowDimension() const;
	I GetColDimension() const;
private:
	I m_dimRow;
	I m_dimCol;
};

template <typename I>
I IndexerRowMajor<I>::numberOfIndices() const
{
	return 2;
}

template <typename I>
IndexerRowMajor<I>::IndexerRowMajor(I dimRow, I dimCol) : m_dimRow(dimRow), m_dimCol(dimCol)
{
}

template <typename I>
I IndexerRowMajor<I>::GetRowDimension() const
{
	return m_dimRow;
}

template <typename I>
I IndexerRowMajor<I>::GetColDimension() const
{
	return m_dimCol;
}

template <typename I>
size_t IndexerRowMajor<I>::ToFlat(std::initializer_list<I> ijk) const
{
	Utilities::MyAssert(2 == ijk.size());

	auto itr = ijk.begin();
	const auto row = *itr;
	++itr;
	const auto col = *itr;

	Utilities::MyAssert(std::min(row, col) >= 0);
	Utilities::MyAssert(row < m_dimRow&& col < m_dimCol);

	return row * m_dimCol + col;
}
