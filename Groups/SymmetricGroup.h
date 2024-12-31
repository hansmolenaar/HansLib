#pragma once

#include "Defines.h"
#include "IFiniteGroup.h"

class SymmetricGroup
{
public:
   static std::unique_ptr<IFiniteGroup> Create(GroupElement size);
};
