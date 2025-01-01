#pragma once

#include "IIndexer.h"
#include <vector>

class IndexerSymmetric :public IIndexer<unsigned int>
{
public:
   IndexerSymmetric(unsigned int dim);
   size_t ToFlat(std::initializer_list<unsigned int> ijk) const override;
   unsigned int numberOfIndices() const override;
   size_t ToFlat(unsigned int, unsigned int) const;

private:
   unsigned int m_dim;
};

