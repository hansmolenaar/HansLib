#include <gtest/gtest.h>

#include "IndexTreeScaled.h"
#include "RefinementPredicates.h"
#include "GeometryBall.h"
#include "IntervalTreeIndexFactory.h"
#include "MeshGeneration2.h"
#include "MeshingStrategy.h"
#include "InitialBoundingboxGenerator.h"
#include "Paraview.h"
#include "PointClose.h"

using namespace MeshGeneration;
using namespace IntervalTree;
using namespace Geometry;

TEST(MeshGeneration2Test, Ball)
{
   const Ball<double, 2> ball(Point2{ 0.5, 0.5 }, 0.5);
   const PointClose<double, 2> areClose;
   const auto initialBbGenerator = InitialBoundingboxGenerator<2>::Create(2.0);
   const RefineRegionToMaxLevel<2> predicate(5, ball, areClose, *initialBbGenerator);
   MeshingStrategy2 strategy(*initialBbGenerator, predicate);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ball, strategy);

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles);
   ASSERT_EQ(1016, vtkData->getNumCells());
   Paraview::Write("MeshGeneration2Test_Ball", *vtkData);

}

