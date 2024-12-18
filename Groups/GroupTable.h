#pragma once

#include "Defines.h"
#include "IFiniteGroup.h"
#include "IIndexer.h"
#include "Permutation.h"

#include <map>

class GroupTable : public IFiniteGroup
{
public:
   static std::unique_ptr<IFiniteGroup> Create(std::unique_ptr<IIndexer<GroupElement>>&, const std::vector< GroupElement>&);
   static std::unique_ptr<IFiniteGroup> CreateFromPermutations( const std::vector<Permutation>& permutations);

   GroupElement getOrder() const override;
   GroupElement getIdentity() const  override;
   GroupElement getInverse(GroupElement) const  override;
   GroupElement operator()(GroupElement, GroupElement) const  override;

private:
   GroupTable(std::unique_ptr<IIndexer<GroupElement>>&, const std::vector< GroupElement>&);

   std::vector<GroupElement>  m_table;
   std::unique_ptr<IIndexer<GroupElement>> m_indexer;
   GroupElement m_order;
   GroupElement m_identity = Permutation::InvalidEntry;
   std::vector<GroupElement> m_inverse;
};
