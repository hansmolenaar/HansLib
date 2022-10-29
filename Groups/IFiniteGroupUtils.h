#pragma once

#include "GroupDefines.h"

class IFiniteGroup;

namespace IFiniteGroupUtils
{
   void CheckValid(const IFiniteGroup&, GroupElement);
   void CheckGroupAxioms(const IFiniteGroup&);
   std::vector<GroupElement> GetElements(const IFiniteGroup&);
}
