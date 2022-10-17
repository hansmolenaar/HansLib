#include "Utils/IndexerRowMajor.h"
#include "Utils/MessageHandler.h"

#include <algorithm>

IndexerRowMajor::IndexerRowMajor(int dimRow, int dimCol)
{
	MessageHandler::Assert(dimRow > 0 && dimCol > 0);
	m_dimRow = dimRow;
	m_dimCol = dimCol;
}

int IndexerRowMajor::ToFlat(std::initializer_list<int> ijk) const
{
	MessageHandler::Assert(2 == ijk.size());

	auto itr = ijk.begin();
	const int row = *itr;
	++itr;
	const int col = *itr;

	MessageHandler::Assert(std::min(row, col) >= 0);
	MessageHandler::Assert(row < m_dimRow && col< m_dimCol);

	return row * m_dimCol + col;
}

int IndexerRowMajor::GetRowDimension() const
{
	return m_dimRow;
}

int IndexerRowMajor::GetColDimension() const
{
	return m_dimCol;
}
