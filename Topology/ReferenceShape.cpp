#include "ReferenceShape.h"

ReferenceShape::ReferenceShape(std::unique_ptr<ITopologicalAdjacencies>&& adjacencies) :
   m_adjacencies(std::move(adjacencies))
{
}

const ITopologicalAdjacencies& ReferenceShape::getAdjacencies() const
{
   return *m_adjacencies;
}