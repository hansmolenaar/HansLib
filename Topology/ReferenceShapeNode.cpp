#include "Topology/ReferenceShapeNode.h"
#include "Utilities/MyException.h"

TopologyDimension ReferenceShapeNode::getMaxTopologyDimension() const
{
   return TopologyDimension::Corner;
}

int ReferenceShapeNode::getCount(TopologyDimension dim) const
{
   return dim == TopologyDimension::Corner ? 1 : 0;
}

const ITopologicalAdjacency& ReferenceShapeNode::getAdjacency(TopologyDimension, TopologyDimension) const
{
   throw  MyException("No way...");
}