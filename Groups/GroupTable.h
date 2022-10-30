#pragma once

#include "Utilities/Defines.h"
#include "IFiniteGroup.h"

#include <map>

class GroupTable //: public IFiniteGroup
{
public:
   GroupTable(const std::vector<std::tuple<GroupElement, GroupElement, GroupElement>>&);
private:
   std::vector<GroupElement>  m_entries;
};
