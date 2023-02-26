#include "SameFiniteGroupPredicate.h"
#include "IFiniteGroupUtils.h"

bool SameFiniteGroupPredicate::operator()(const IFiniteGroup& other) const
{
   if (group.getOrder() != other.getOrder()) return false;
   if (group.getIdentity() != other.getIdentity()) return false;
   const int order = group.getOrder();
   const auto& elements = IFiniteGroupUtils::GetElements(group);
   for (auto elm : elements)
   {
      if (group.getInverse(elm) != other.getInverse(elm)) return false;
      for (auto elm2 : elements)
      {
         if (group(elm, elm2) != other(elm, elm2)) return false;
      }
   }
   return true;
}