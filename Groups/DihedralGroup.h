#pragma once

#include "IFiniteGroup.h"
#include <memory>

class DihedralGroup : public IFiniteGroup
{
public:
   static std::unique_ptr<IFiniteGroup> Create(GroupElement);

   GroupElement getOrder() const override;
   GroupElement getIdentity() const override;
   GroupElement getInverse(GroupElement) const override;
   GroupElement operator()(GroupElement, GroupElement) const override;

private:
   DihedralGroup(std::unique_ptr<IFiniteGroup>&);
   std::unique_ptr<IFiniteGroup> m_groupTable;
};