#pragma once

#include "Assert.h"
#include "BoundsCheck.h"
#include "Defines.h"

#include <span>
#include <boost/container/small_vector.hpp>


static constexpr size_t MULTI_INDEX_MAX_SMALL_VECTOR = 6;

template <typename T>
class MultiIndex
{
public:

   static MultiIndex<T> Create(std::span<const T> dimensions);

   size_t getNumDimensions() const;
   size_t getFlatSize() const;
   T at(size_t) const;

   std::vector<T> toMultiplet(size_t) const;

   void toMultiplet(size_t, std::span<T>) const;
   size_t toFlat(std::span<const T>) const;

private:

   explicit  MultiIndex(boost::container::small_vector<T, MULTI_INDEX_MAX_SMALL_VECTOR> dimensions);
   boost::container::small_vector<T, MULTI_INDEX_MAX_SMALL_VECTOR> m_dimensions;
   size_t m_flatLength;
   boost::container::small_vector<T, MULTI_INDEX_MAX_SMALL_VECTOR> m_factors;
   BoundsCheck<T> m_checkFlat;
   boost::container::small_vector<BoundsCheck<T>, MULTI_INDEX_MAX_SMALL_VECTOR> m_checkIndex;
};
