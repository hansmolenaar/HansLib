#include "Utilities/IndexerSymmetric.h"
#include "Utilities/MessageHandler.h"

#include <tuple>

IndexerSymmetric::IndexerSymmetric(int dim) : m_dim(dim)
{
	MessageHandler::Assert(dim > 0);
}

int IndexerSymmetric::ToFlat(std::initializer_list<int> ijk) const
{
	MessageHandler::Assert(2 == ijk.size());
	return ToFlat(*ijk.begin(), *(ijk.begin()+1));
}


int IndexerSymmetric::ToFlat(int row, int col) const
{
	MessageHandler::Assert(row >= 0 && col >= 0 && row < m_dim && col < m_dim);
	if (row < col) 
	{
		return col * (col + 1) / 2 + row;
	}
	else
	{
		return row * (row + 1) / 2 + col;
	}
}

int IndexerSymmetric::numberOfIndices() const
{
	return 2;
}