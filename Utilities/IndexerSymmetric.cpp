#include "IndexerSymmetric.h"
#include "MyAssert.h"

IndexerSymmetric::IndexerSymmetric(unsigned int dim) : m_dim(dim)
{
    Utilities::MyAssert(dim > 0);
}

size_t IndexerSymmetric::ToFlat(std::initializer_list<unsigned int> ijk) const
{
    Utilities::MyAssert(2 == ijk.size());
    return ToFlat(*ijk.begin(), *(ijk.begin() + 1));
}

size_t IndexerSymmetric::ToFlat(unsigned int row, unsigned int col) const
{
    Utilities::MyAssert(row >= 0 && col >= 0 && row < m_dim && col < m_dim);
    if (row < col)
    {
        return col * (col + 1) / 2 + row;
    }
    else
    {
        return row * (row + 1) / 2 + col;
    }
}

unsigned int IndexerSymmetric::numberOfIndices() const
{
    return 2;
}