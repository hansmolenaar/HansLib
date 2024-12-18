#include "GroupTable.h"
#include "MyAssert.h"
#include "ToString.h"
#include "IFiniteGroupUtils.h"
#include "IndexerRowMajor.h"

#include <cmath>

std::unique_ptr<IFiniteGroup> GroupTable::Create(std::unique_ptr<IIndexer<GroupElement>>& indexer, const std::vector< GroupElement>& elements)
{
   std::unique_ptr<IFiniteGroup> result(new GroupTable(indexer, elements));
   IFiniteGroupUtils::CheckGroupAxioms(*result);
   return result;
}

std::pair<std::unique_ptr<IFiniteGroup>, std::vector<Permutation>> GroupTable::GeneratedBy(const std::vector<Permutation>& permutationsIn)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer;
   if (permutationsIn.empty()) return { Create(indexer, std::vector< GroupElement>{}), std::vector<Permutation> {} };

   std::map<Permutation, GroupElement> permLookup;
   for (const auto& perm : permutationsIn)
   {
      Permutation permPow = perm;
      while (true)
      {
         if (!permLookup.emplace(permPow, static_cast<Permutation::Entry>(permLookup.size())).second) break;
         permPow = permPow * perm;
      }

      const Permutation inverse = perm.getInverse();
      permPow = inverse;
      while (true)
      {
         if (!permLookup.emplace(permPow, static_cast<Permutation::Entry>(permLookup.size())).second) break;
         permPow = permPow * inverse;
      }
   }

   if (permLookup.size() >= std::numeric_limits<GroupElement>::max()) throw MyException("CreateFromPermutations too large: " + std::to_string(permutationsIn.size()));
   const auto order = static_cast<GroupElement>(permLookup.size());

   std::vector<Permutation> permutations(order, Permutation::CreateTrivial(0));
   for (const auto& itr : permLookup)
   {
      permutations.at(itr.second) = itr.first;
   }

   indexer = std::make_unique< IndexerRowMajor<GroupElement>>(order, order);
   std::vector<GroupElement> table(order * order, GroupElementInvalid);

   for (GroupElement n0 = 0; n0 < permutations.size(); ++n0)
   {
      for (GroupElement n1 = 0; n1 < permutations.size(); ++n1)
      {
         const auto composition = permutations.at(n0) * permutations.at(n1);
         const auto pos = indexer->ToFlat({ n0, n1 });
         table.at(pos) = permLookup.at(composition);
      }
   }

   if (str::any_of(table, [](GroupElement g) {return g == GroupElementInvalid; }))
   {
      throw MyException("CreateFromPermutations incomplete");
   }
   return { Create(indexer, table), permutations };
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