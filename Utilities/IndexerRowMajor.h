#pragma once

#include "IIndexer.h"
#include "MessageHandler.h"
#include <vector>

template <typename I>
class IndexerRowMajor :public IIndexer<I>
{
public:
	IndexerRowMajor(I dimRow, I dimCol);
	I ToFlat(std::initializer_list<I> ijk) const override;
	int numberOfIndices() const override;
	I GetRowDimension() const;
	I GetColDimension() const;
private:
	I m_dimRow;
	I m_dimCol;
};

template <typename I>
int IndexerRowMajor<I>::numberOfIndices() const
{
	return 2;
}

template <typename I>
IndexerRowMajor<I>::IndexerRowMajor(I dimRow, I dimCol) : m_dimRow(dimRow), m_dimCol(dimCol)
{
	MessageHandler::Assert(dimRow > 0 && dimCol > 0);
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
I IndexerRowMajor<I>::ToFlat(std::initializer_list<I> ijk) const
{
	MessageHandler::Assert(2 == ijk.size());

	auto itr = ijk.begin();
	const auto row = *itr;
	++itr;
	const auto col = *itr;

	MessageHandler::Assert(std::min(row, col) >= 0);
	MessageHandler::Assert(row < m_dimRow&& col < m_dimCol);

	return row * m_dimCol + col;
}
