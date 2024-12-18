#include "DihedralGroup.h"
#include "GroupTable.h"
#include "IndexerRowMajor.h"

DihedralGroup::DihedralGroup(std::unique_ptr<IFiniteGroup>& groupTable) :
   m_groupTable(std::move(groupTable))
{
}

std::unique_ptr<IFiniteGroup> DihedralGroup::Create(GroupElement numVertices)
{
   const GroupElement tableDim = 2 * numVertices;
   std::unique_ptr<IIndexer<GroupElement>> indexer = std::make_unique< IndexerRowMajor<GroupElement>>(tableDim, tableDim);
   std::vector<GroupElement> table(4 * numVertices * numVertices, GroupElementInvalid);

   // Ordering: first n rotations, then n refelections
   for (GroupElement n0 = 0; n0 < numVertices; ++n0)
   {
      for (GroupElement n1 = 0; n1 < numVertices; ++n1)
      {
         table.at(indexer->ToFlat({ n0, n1 })) = (n0 + n1) % numVertices;
         table.at(indexer->ToFlat({ numVertices + n0, numVertices + n1 })) = (numVertices + n0 - n1) % numVertices;
         table.at(indexer->ToFlat({ numVertices + n0, n1 })) = numVertices + (numVertices + n0 - n1) % numVertices;
         table.at(indexer->ToFlat({ n0, numVertices + n1 })) = numVertices + (numVertices + n1 + n0) % numVertices;
      }
   }

   auto groupTable = GroupTable::Create(indexer, table);
   return std::unique_ptr<IFiniteGroup>(new DihedralGroup(groupTable));
}

GroupElement DihedralGroup::getOrder() const
{
   return m_groupTable->getOrder();
}

GroupElement DihedralGroup::getIdentity() const
{
   return m_groupTable->getIdentity();
}

GroupElement DihedralGroup::getInverse(GroupElement elm) const
{
   return m_groupTable->getInverse(elm);
}

GroupElement DihedralGroup::operator()(GroupElement elm1, GroupElement elm2) const
{
   return (*m_groupTable)(elm1, elm2);
}