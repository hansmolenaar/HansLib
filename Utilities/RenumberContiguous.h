#pragma once

#include "Defines.h"
#include "MyException.h"
#include <algorithm>
#include <optional>
#include <sstream>
#include <unordered_map>

namespace Utilities
{
   template <class Tin, class Tcontiguous>
   class RenumberContiguous
   {
   public:
      template<class FwdItr>
      RenumberContiguous(FwdItr first, FwdItr last)
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

      std::optional<Tcontiguous> toContiguous(const Tin& value) const;
      const Tin& at(Tcontiguous idx) const;
      Tcontiguous size() const { return m_toContiguous.size(); }

   private:
      std::vector<Tin> m_original;
      std::unordered_map<Tin, Tcontiguous> m_toContiguous;
   };

#if 0
   template <class Tin, class Tcontiguous>
   template<class FwdItr>
   RenumberContiguous<class Tin, class Tcontiguous>::RenumberContiguous(FwdItr first, FwdItr last) : m_original(first, last)
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
#endif

   template <class Tin,class Tcontiguous>
   const Tin& RenumberContiguous<Tin, Tcontiguous>::at(Tcontiguous idx) const
   {
      return m_original.at(idx);
   }

   template <class Tin, class Tcontiguous>
   std::optional<Tcontiguous> RenumberContiguous<Tin, Tcontiguous>::toContiguous(const Tin& value) const
   {
      const auto found = m_toContiguous.find(value);
      if (found != m_toContiguous.end())
      {
         return { found->second };
      }
      return {};
   }
}
