#pragma once

#include "IIndexer.h"
#include "MyAssert.h"

#include <vector>

template <typename I> class IndexerSymmetric : public IIndexer<I>
{
  public:
    explicit IndexerSymmetric(I dim);
    size_t toFlat(std::initializer_list<I> ijk) const override;
    I getNumberOfIndices() const override;
    size_t getFlatSize() const override;
    size_t ToFlat(I, I) const;

  private:
    I m_dim;
};

template <typename I> IndexerSymmetric<I>::IndexerSymmetric(I dim) : m_dim(dim)
{
    Utilities::MyAssert(dim > 0);
}

template <typename I> size_t IndexerSymmetric<I>::toFlat(std::initializer_list<I> ijk) const
{
    Utilities::MyAssert(2 == ijk.size());
    return ToFlat(*ijk.begin(), *(ijk.begin() + 1));
}

template <typename I> size_t IndexerSymmetric<I>::ToFlat(I row, I col) const
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

template <typename I> I IndexerSymmetric<I>::getNumberOfIndices() const
{
    return static_cast<I>(2);
}

template <typename I> size_t IndexerSymmetric<I>::getFlatSize() const
{
    return m_dim * (m_dim + 1) / 2;
}
