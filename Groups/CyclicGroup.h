#pragma once

#include "IFiniteGroup.h"
#include "HLUtils/Defines.h"


class CyclicGroup : public IFiniteGroup
{
public:
   static std::unique_ptr<CyclicGroup> Create(int);

    int getOrder() const override;
    GroupElement getIdentity() const override;
    GroupElement getInverse(GroupElement) const override;
    GroupElement operator()(GroupElement, GroupElement) const override;

private:
   CyclicGroup(int);
   int m_order;
};

