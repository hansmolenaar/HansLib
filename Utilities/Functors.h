#pragma once

#include <type_traits>

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
}
