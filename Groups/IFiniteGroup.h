#pragma once

#include "GroupDefines.h"

class IFiniteGroup
{
public:
   virtual ~IFiniteGroup() = default;
   virtual int getOrder() const = 0;
   virtual GroupElement getIdentity() const = 0;
   virtual GroupElement getInverse(GroupElement) const = 0;
   virtual GroupElement operator()(GroupElement, GroupElement) const = 0;
};
