#pragma once

#include "Defines.h"
#include "IFiniteGroup.h"
#include "IIndexer.h"
#include "Permutation.h"
#include "IndexerRowMajor.h"

#include <map>

class GroupTable : public IFiniteGroup
{
public:
   template<typename BinOp>
   static std::unique_ptr<IFiniteGroup> CreateUsingBinaryOperator(const std::vector< GroupElement>&, BinOp binop, bool checkAssociativity);

   static std::unique_ptr<IFiniteGroup> Create(std::unique_ptr<IIndexer<GroupElement>>&, const std::vector< GroupElement>&, bool checkAssociativity = true);
   static std::pair<std::unique_ptr<IFiniteGroup>, std::vector<Permutation>> GeneratedBy(const std::vector<Permutation>& permutations);

   GroupElement getOrder() const override;
   GroupElement getIdentity() const  override;
   GroupElement getInverse(GroupElement) const  override;
   GroupElement operator()(GroupElement, GroupElement) const  override;

private:
   GroupTable(std::unique_ptr<IIndexer<GroupElement>>&, const std::vector< GroupElement>&);

   std::vector<GroupElement>  m_table;
   std::unique_ptr<IIndexer<GroupElement>> m_indexer;
   GroupElement m_order;
   GroupElement m_identity = GroupElementInvalid;
   std::vector<GroupElement> m_inverse;
};

template<typename BinOp>
static std::unique_ptr<IFiniteGroup> GroupTable::CreateUsingBinaryOperator(const std::vector<GroupElement>& elements, BinOp binop, bool checkAssociativity)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer;
   if (elements.empty()) return  Create(indexer, elements);

   if (elements.size() >= std::numeric_limits<GroupElement>::max()) throw MyException("CreateUsingBinaryOperator too large: " + std::to_string(elements.size()));
   const auto order = static_cast<GroupElement>(elements.size());

   std::vector<GroupElement> table(order * order, GroupElementInvalid);
   indexer = std::make_unique< IndexerRowMajor<GroupElement>>(order, order);
   if (order == 1)
   {
      table[0] = 0;
      return  Create(indexer, table);
   }

   return {};
}