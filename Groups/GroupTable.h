#pragma once

#include "Utilities/Defines.h"
#include "IFiniteGroup.h"
#include "Utilities/IIndexer.h"

#include <map>

class GroupTable : public IFiniteGroup
{
public:
   static std::unique_ptr<IFiniteGroup> Create(std::unique_ptr<IIndexer<GroupElement>>&, const std::vector< GroupElement>&);

   int getOrder() const override;
   GroupElement getIdentity() const  override;
   GroupElement getInverse(GroupElement) const  override;
   GroupElement operator()(GroupElement, GroupElement) const  override;

private:
   GroupTable(std::unique_ptr<IIndexer<GroupElement>>&, const std::vector< GroupElement>&);

   std::vector<GroupElement>  m_table;
   std::unique_ptr<IIndexer<GroupElement>> m_indexer;
   int m_order;
   GroupElement m_identity = -1;
   std::vector<GroupElement> m_inverse;
};
