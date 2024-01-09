#include "MultiIndex.h"

template MultiIndex<int>;
template MultiIndex<size_t>;

template <typename T>
MultiIndex<T>::MultiIndex(boost::container::small_vector<T, MULTI_INDEX_MAX_SMALL_VECTOR>&& dimensions) :
   m_dimensions(std::move(dimensions))

{
   Utilities::MyAssert(!m_dimensions.empty());

   m_flatLength = 1;
   m_factors.reserve(m_dimensions.size());
   for (auto d : m_dimensions)
   {
      m_checkIndex.push_back(BoundsCheck<T>::Create(0, d - 1));
      m_factors.push_back(static_cast<T>(m_flatLength));
      m_flatLength *= d;
   }
   m_checkFlat = BoundsCheck<T>::Create(0, static_cast<T>(m_flatLength) - 1);
}

template <typename T>
MultiIndex<T>  MultiIndex<T>::Create(std::span<const T> dimensions)
{
   boost::container::small_vector<T, MULTI_INDEX_MAX_SMALL_VECTOR> myDimensions(dimensions.size());
   str::copy(dimensions, myDimensions.begin());
   return MultiIndex<T>(std::move(myDimensions));
}

template <typename T>
size_t MultiIndex<T>::getNumDimensions() const
{
   return m_dimensions.size();
}

template <typename T>
size_t MultiIndex<T>::getFlatSize() const
{
   return m_flatLength;
}

template <typename T>
T MultiIndex<T>::at(size_t n) const
{
   return m_dimensions.at(n);
}

template <typename T>
void MultiIndex<T>::toMultiplet(size_t flat, std::span<T> multiplet) const
{
   m_checkFlat.check(static_cast<T>(flat));
   if (multiplet.size() != getNumDimensions())
   {
      std::string msg = "MultiIndex<T>::toMultiplet incorrect input dimension ";
      msg += std::to_string(multiplet.size());
      msg += " expeced " + std::to_string(getNumDimensions());
      throw MyException(msg);
   }

   auto* data = multiplet.data();

   // Loop backwards!
   for (int d = static_cast<int>(getNumDimensions() - 1); d >= 0; --d)
   {
      T index = static_cast<T>(flat) / m_factors[d];
      *data = index;
      ++data;
      flat -= index * m_factors[d];
   }
   std::reverse(multiplet.begin(), multiplet.end());
}


template <typename T>
size_t MultiIndex<T>::toFlat(std::span<const T> multiplet) const
{
   Utilities::MyAssert(multiplet.size() == getNumDimensions());
   size_t result = 0;
   size_t count = 0;
   for (auto indx : multiplet)
   {
      m_checkIndex.at(count).check(indx);
      result += indx * m_factors[count];
      ++count;
   }

   return result;
}