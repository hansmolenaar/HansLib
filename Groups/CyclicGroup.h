#pragma once

#include "IFiniteGroup.h"
#include <memory>

class CyclicGroup : public IFiniteGroup
{
  public:
    static std::unique_ptr<CyclicGroup> Create(GroupElement);

    GroupElement getOrder() const override;
    GroupElement getIdentity() const override;
    GroupElement getInverse(GroupElement) const override;
    GroupElement operator()(GroupElement, GroupElement) const override;

  private:
    CyclicGroup(GroupElement);
    GroupElement m_order;
};
