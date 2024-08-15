#pragma once

#include "MyAssert.h"
#include <vector>
#include <span>
#include <boost/container/static_vector.hpp>

namespace Utilities
{
   template<typename T>
   const T& Single(const std::vector<T>&);

   template<typename T>
   T& Single(std::vector<T>&);

   template<typename T, int N>
   const T& Single(const boost::container::static_vector<T, N>&);

   template<typename T>
   const T& Single(const std::span<const T>&);
}


template<typename T>
const T& Utilities::Single(const std::vector<T>& container)
{
   MyAssert(container.size() == 1);
   return container.front();
}

template<typename T>
T& Utilities::Single(std::vector<T>& container)
{
   MyAssert(container.size() == 1);
   return container.front();
}


template<typename T, int N>
const T& Utilities::Single(const boost::container::static_vector<T, N>& container)
{
   MyAssert(container.size() == 1);
   return container.front();
}

template<typename T>
const T& Utilities::Single(const std::span<const T>& container)
{
   MyAssert(container.size() == 1);
   return container.front();
}