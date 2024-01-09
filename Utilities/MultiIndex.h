#pragma once

#include "Assert.h"
#include "BoundsCheck.h"
#include "Defines.h"
#include <span>

template <typename T>
class MultiIndex
{
public:

   static MultiIndex<T> Create(std::vector<T>&& dimensions);

   size_t getNumDimensions() const;
   size_t getFlatSize() const;
   T at(size_t) const;
   std::vector<T> toMultiplet(size_t) const;
   size_t toFlat(std::span<const T>) const;

private:
   explicit  MultiIndex(std::vector<T> dimensions);
   std::vector<T> m_dimensions;
   size_t m_flatLength;
   std::vector<T> m_factors;
   BoundsCheck<T> m_checkFlat;
   std::vector<BoundsCheck<T>> m_checkIndex;
};
