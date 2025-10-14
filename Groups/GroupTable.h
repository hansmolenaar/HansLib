#pragma once

#include "Defines.h"
#include "IFiniteGroup.h"
#include "IIndexer.h"
#include "IndexerRowMajor.h"
#include "Permutation.h"

class GroupTable : public IFiniteGroup
{
public:
   template<typename T, typename BinOp>
   static std::unique_ptr<IFiniteGroup> CreateUsingBinaryOperator(const std::vector< T>&, BinOp binop, bool checkAssociativity);

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

template<typename T, typename BinOp>
std::unique_ptr<IFiniteGroup> GroupTable::CreateUsingBinaryOperator(const std::vector<T>& elements, BinOp binop, bool checkAssociativity)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer;
   if (elements.empty()) return  Create(indexer, std::vector<GroupElement>{});

   if (elements.size() >= std::numeric_limits<GroupElement>::max()) throw MyException("CreateUsingBinaryOperator too large: " + std::to_string(elements.size()));
   const auto order = SafeCastToGroupElement(elements.size());

   std::vector<GroupElement> table(order * order, GroupElementInvalid);
   indexer = std::make_unique< IndexerRowMajor<GroupElement>>(order, order);
   for (GroupElement n0 = 0; n0 < order; ++n0)
   {
      for (GroupElement n1 = 0; n1 < order; ++n1)
      {
         const auto composition = binop(elements.at(n0), elements.at(n1));
         const auto found = str::find(elements, composition);
         if (found == elements.end())
         {
            throw MyException("CreateUsingBinaryOperator missing permutation!!");
         }
         const auto pos = indexer->ToFlat({ n0, n1 });
         const auto groupElement = SafeCastToGroupElement(std::distance(elements.begin(), found));
         table.at(pos) = groupElement;
      }
   }

   if (str::any_of(table, [](GroupElement g) {return g == GroupElementInvalid; }))
   {
      throw MyException("CreateUsingBinaryOperator incomplete");
   }

   return Create(indexer, table, checkAssociativity);
}
