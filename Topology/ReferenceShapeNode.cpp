#include "Topology/ReferenceShapeNode.h"
#include "Utilities/MyException.h"

class NodeAdjacencies : public ITopologicalAdjacencies
{
public:
   TopologyDimension getMaxTopologyDimension() const override
   {
      return TopologyDimension::Corner;
   }

   int getCount(TopologyDimension dim) const override
   {
      return dim == TopologyDimension::Corner ? 1 : 0;
   }

   const ITopologicalAdjacency& getAdjacency(TopologyDimension, TopologyDimension) const override
   {
      throw  MyException("No way...");
   }
};

namespace
{
   const NodeAdjacencies Instance;
}

const ITopologicalAdjacencies& ReferenceShapeNode::getAdjacencies() const
{
   return Instance;
}