#pragma once

#include "Defines.h"
#include "MyException.h"
#include <algorithm>
#include <sstream>
#include <unordered_map>

namespace Utilities
{
   template <typename Tcontiguous, typename C>
   class RenumberContiguous
   {
   public:
      explicit RenumberContiguous(const C& numbersIn);
      Tcontiguous toContiguous(typename C::value_type value) const;
      typename C::value_type operator[](Tcontiguous idx) const;
   private:
      std::vector<typename C::value_type> m_original;
      std::unordered_map<typename C::value_type, Tcontiguous> m_toContiguous;
   };

   template <typename Tcontiguous, typename C>
   RenumberContiguous<Tcontiguous, C>::RenumberContiguous(const C& numbersIn) : m_original(numbersIn.begin(), numbersIn.end())
   {
      for (Tcontiguous n = 0; const auto & org : m_original)
      {
         const auto retval = m_toContiguous.emplace(org, n);
         if (!retval.second)
         {
            std::ostringstream os;
            os << "RenumberContiguous::RenumberContiguous duplicate entry " << org;
            throw MyException(os.str());
         }
         ++n;
      }
   }

   template <typename Tcontiguous, typename C>
   typename C::value_type RenumberContiguous<Tcontiguous, C>::operator[](Tcontiguous idx) const
   {
      return m_original.at(idx);
   }

   template <typename Tcontiguous, typename C>
   Tcontiguous RenumberContiguous<Tcontiguous, C>::toContiguous(typename C::value_type value) const
   {
      const auto found = m_toContiguous.find(value);
      if (found == m_toContiguous.end())
      {
         std::ostringstream os;
         os << "RenumberContiguous<CTcontiguous, C>::toContiguous unknown value " << value;
         throw MyException(os.str());
      }
      return found->second;
   }
}
