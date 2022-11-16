#pragma once

#include "GroupDefines.h"
#include <vector>

class IFiniteGroup;

namespace IFiniteGroupUtils
{
   void CheckValid(const IFiniteGroup&, GroupElement);
   void CheckGroupAxioms(const IFiniteGroup&);
   std::vector<GroupElement> GetElements(const IFiniteGroup&);
}
