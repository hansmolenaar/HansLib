#include <gtest/gtest.h>

#include "IndexTreeScaled.h"
#include "RefinementPredicates.h"
#include "GeometrySphere.h"
#include "IntervalTreeIndexFactory.h"
#include "MeshGeneration2.h"
#include "MeshingStrategy.h"
#include "InitialBoundingboxGenerator.h"
#include "Paraview.h"

using namespace MeshGeneration;
using namespace IntervalTree;
using namespace Geometry;

TEST(MeshGeneration2Test, Sphere)
{
   const Sphere<double, 2> sphere(Point2{ 0.5, 0.5 }, 0.5);
   const RefineRegionToMaxLevel<2> predicate(5, sphere);
   const auto initialBbGenerator = InitialBoundingboxGenerator<2>::Create(1.1);
   MeshingStrategy2 strategy(*initialBbGenerator, predicate);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(strategy);

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles);
   //ASSERT_EQ(2, vtkData->getNumCells());
   //ASSERT_EQ(4, vtkData->getNumNodes());
   Paraview::Write("MeshGeneration2Test_Spere", *vtkData);

}

