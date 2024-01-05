#include "MeshGeneration2.h"
#include "IntervalTreeBalance.h"
#include "Defines.h"   
#include "BoundingBox.h"

using namespace Geometry;
using namespace IntervalTree;

IndexTreeToSimplices2::Triangles MeshGeneration2::GenerateBaseTriangulation(const IGeometryRegion<double, 2>& region, MeshingStrategy2& strategy)
{
   IndexTree<2> tree;
   tree.refineUntilReady(strategy.getRefinementPredicate());
   IntervalTree::Balance(tree);
   return IndexTreeToSimplices2::Create(tree);
}