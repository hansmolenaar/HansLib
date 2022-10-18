#pragma once

#include "IFiniteGroup.h"
class IFinitePointGroupAction;

class IFinitePointGroup : public IFiniteGroup<const IFinitePointGroupAction&>
{
public:
   virtual ~IFinitePointGroup() {};
};
