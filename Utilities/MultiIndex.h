#pragma once

#include "Utilities/Assert.h"
#include "Utilities/BoundsCheck.h"
#include "Utilities//Defines.h"
#include <span>

template <typename T>
class MultiIndex
{
public:

   static MultiIndex<T> Create(std::vector<T>&& dimensions);

   size_t getNumDimensions() const;
   T getFlatSize() const;
   T at(size_t) const;
   std::vector<T> toMultiplet(T) const;
   T toFlat(std::span<const T>) const;

private:
   explicit  MultiIndex(std::vector<T> dimensions);
   std::vector<T> m_dimensions;
   T m_flatLength;
   std::vector<T> m_factors;
   BoundsCheck<T> m_checkFlat;
   std::vector<BoundsCheck<T>> m_checkIndex;
};

template <typename T>
MultiIndex<T>::MultiIndex(std::vector<T> dimensions) :
   m_dimensions(std::move(dimensions))

{
   Utilities::Assert(!m_dimensions.empty());
   const auto checker = BoundsCheck<T>::CreateIsPositive();

   m_flatLength = 1;
   m_factors.reserve(m_dimensions.size());
   for (auto d : m_dimensions)
   {
      checker.check(d);
      m_checkIndex.push_back(BoundsCheck<T>::Create(0, d - 1));
      m_factors.push_back(m_flatLength);
      m_flatLength *= d;
   }
   checker.check(m_flatLength);
   m_checkFlat = BoundsCheck<T>::Create(0, m_flatLength - 1);
}

template <typename T>
MultiIndex<T>  MultiIndex<T>::Create(std::vector<T>&& dimensions)
{
   return MultiIndex<T>(std::move(dimensions));
}

template <typename T>
size_t MultiIndex<T>::getNumDimensions() const
{
   return m_dimensions.size();
}

template <typename T>
T MultiIndex<T>::getFlatSize() const
{
   return m_flatLength;
}

template <typename T>
T MultiIndex<T>::at(size_t n) const
{
   return m_dimensions.at(n);
}

template <typename T>
std::vector<T> MultiIndex<T>::toMultiplet(T flat) const
{
   m_checkFlat.check(flat);
   std::vector<T> result;
   result.reserve(getNumDimensions());

   // Loop backwards!
   for (auto d = getNumDimensions() - 1; d >= 0; --d)
   {
      T index = flat / m_factors[d];
      result.push_back(index);
      flat -= index * m_factors[d];
      if (d == 0) break; // break: d is unsigned
   }
   std::reverse(result.begin(), result.end());
   return result;
}


template <typename T>
T MultiIndex<T>::toFlat(std::span<const T> multiplet) const
{
   Utilities::Assert(multiplet.size() == getNumDimensions());
   T result = 0;
   size_t count = 0;
   for (auto indx : multiplet)
   {
      m_checkIndex.at(count).check(indx);
      result += indx * m_factors[count];
      ++count;
   }

   return result;
}