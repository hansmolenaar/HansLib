#pragma once

#include <type_traits>
#include <algorithm>
#include <numeric>

namespace Functors
{
   template<typename T>
   struct IsEqualTo
   {
      T Expect;
      bool operator()(const T& actual) const { return actual == Expect; }
   };

   template< typename TDerived>
   struct IsOfDerivedType
   {
      template<typename TBase>
      bool operator()(const TBase* base ) const
      { 
         return dynamic_cast<const TDerived*>(base) != nullptr;
      }
   private:
   };

   struct AreClose
   {
      double RelTolerance = 1.0e-12;
      double AbsTolerance = 1.0e-100;
      bool operator()(double x, double y) const
      {
         const double maxabs = std::max(std::abs(x), std::abs(y));
         if (maxabs < AbsTolerance) return true;
         return std::abs(x - y) <= maxabs * RelTolerance;
      }
   };
}
