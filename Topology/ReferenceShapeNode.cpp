#include "Topology/ReferenceShapeNode.h"
#include "Utilities/MyException.h"
#include "Utilities/BoundsCheck.h"
namespace
{
   const auto Checker = BoundsCheck<TopologyDimension>::Create(TopologyDimension::Corner, TopologyDimension::Corner);
}

class NodeAdjacencies : public ITopologicalAdjacencies
{
public:
   TopologyDimension getMaxTopologyDimension() const override
   {
      return TopologyDimension::Corner;
   }

   int getCount(TopologyDimension dim) const override
   {
      Checker.check(dim);
      return 1;
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