#include "GroupTable.h"
#include "Utilities/Assert.h"
#include "Utilities/ToString.h"
#include "IFiniteGroupUtils.h"

#include <cmath>

std::unique_ptr<IFiniteGroup> GroupTable::Create(std::unique_ptr<IIndexer<GroupElement>>& indexer, const std::vector< GroupElement>& elements)
{
   std::unique_ptr<IFiniteGroup> result(new GroupTable(indexer, elements));
   IFiniteGroupUtils::CheckGroupAxioms(*result);
   return result;
}

GroupTable::GroupTable(std::unique_ptr<IIndexer<GroupElement>>& indexer, const std::vector< GroupElement>& elements) :
   m_table(elements), m_indexer(std::move(indexer))
{
   m_order = static_cast<int>(std::round(std::sqrt(m_table.size())));
   if (m_order * m_order != m_table.size())
   {
      throw MyException("GroupTable::GroupTable() size incorrect: " + ToString(m_table.size()));
   }

   for (GroupElement elm = 0; elm < m_order; ++elm)
   {
      if (((*this)(0, elm) == 0))
      {
         m_identity = elm;
      }
   }
   if (m_identity == -1)
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

int GroupTable::getOrder() const
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