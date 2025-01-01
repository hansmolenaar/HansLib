#pragma once

#include <initializer_list>

template <typename I>
class IIndexer
{
public:
   virtual	~IIndexer() = default;
   virtual I numberOfIndices() const = 0;
   virtual size_t ToFlat(std::initializer_list<I> ijk) const = 0;
};

