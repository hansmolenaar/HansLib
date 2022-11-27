#include "ReferenceshapePolyline.h"
#include "TopologicalAdjacency.h"
#include "Utilities/BoundsCheck.h"

#include <map>

const ReferenceshapePolyline& ReferenceshapePolyline::get(int numVertices)
{
   static std::map<int, std::unique_ptr<ITopologicalAdjacencies>> m_adjacencies;
   BoundsCheck<int>::CreateLowerBound(0).check(numVertices);
   throw MyException("Not implemented");
}