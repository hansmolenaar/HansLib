#include "GroupTable.h"
#include "MyAssert.h"
#include "ToString.h"
#include "IFiniteGroupUtils.h"
#include "IndexerRowMajor.h"
#include "PermutationUtils.h"
#include "MultiIndex.h"

#include <cmath>
#include <set>

namespace
{
} // namespace

std::unique_ptr<IFiniteGroup> GroupTable::Create(std::unique_ptr<IIndexer<GroupElement>>& indexer, const std::vector< GroupElement>& elements, bool checkAssociativity)
{
   std::unique_ptr<IFiniteGroup> result(new GroupTable(indexer, elements));
   IFiniteGroupUtils::CheckGroupAxioms(*result, checkAssociativity);
   return result;
}

std::pair<std::unique_ptr<IFiniteGroup>, std::vector<Permutation>> GroupTable::GeneratedBy(const std::vector<Permutation>& permutationsIn)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer;
   if (permutationsIn.empty()) return { Create(indexer, std::vector< GroupElement>{}), std::vector<Permutation> {} };

   const std::set<Permutation> generated = PermutationUtils::generateAllPowerCombinations(permutationsIn);

   if (generated.size() >= std::numeric_limits<GroupElement>::max()) throw MyException("CreateFromPermutations too large: " + std::to_string(generated.size()));
   const auto order = static_cast<GroupElement>(generated.size());

   std::vector<Permutation> permutations;
   permutations.reserve(order);
   for (const auto& itr : generated)
   {
      permutations.emplace_back(itr);
   }

   // For permutations we don't have to check the associativity
   constexpr bool checkAssociativity = false;
   auto compose = [](const Permutation& perm1, const Permutation& perm0) {return perm1 * perm0; };
   return { CreateUsingBinaryOperator<Permutation, decltype(compose)>(permutations, compose, checkAssociativity), permutations };
}

GroupTable::GroupTable(std::unique_ptr<IIndexer<GroupElement>>& indexer, const std::vector< GroupElement>& elements) :
   m_table(elements), m_indexer(std::move(indexer))
{
   m_order = static_cast<GroupElement>(std::round(std::sqrt(m_table.size())));
   if (m_order == 0) return;

   if (m_order * m_order != m_table.size())
   {
      throw MyException("GroupTable::GroupTable() size incorrect: " + ToString(m_table.size()));
   }

   for (GroupElement elm = 0; elm < m_order && m_identity == GroupElementInvalid; ++elm)
   {
      m_identity = elm; // Try...
      for (GroupElement tst = 0; tst < m_order; ++tst)
      {
         if (((*this)(tst, elm) != tst))
         {
            m_identity = GroupElementInvalid;
            break;
         }
      }
   }
   if (m_identity == GroupElementInvalid)
   {
      throw MyException("GroupTable::GroupTable no identity");
   }

   m_inverse = std::vector<GroupElement>(m_order, -1);
   for (GroupElement elm1 = 0; elm1 < m_order; ++elm1)
   {
      for (GroupElement elm2 = 0; elm2 < m_order; ++elm2)
      {
         if (((*this)(elm1, elm2) == m_identity))
         {
            m_inverse.at(elm1) = elm2;
         }
      }
   }
}

GroupElement GroupTable::getOrder() const
{
   return m_order;
}

GroupElement GroupTable::getIdentity() const
{
   return m_identity;
}

GroupElement GroupTable::getInverse(GroupElement elm) const
{
   return m_inverse.at(elm);
}


GroupElement GroupTable::operator()(GroupElement elm1, GroupElement elm2) const
{
   const auto pos = m_indexer->ToFlat({ elm1, elm2 });
   return m_table.at(pos);
}