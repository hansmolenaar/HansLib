#include <gtest/gtest.h>

#include "MeshQuality.h"
#include "UniquePointCollectionBinning.h"
#include "PointClose.h"
#include "TrianglesNodes.h"
#include "CellQuality2.h"

using namespace MeshGeneration;
using namespace Topology;
using namespace Utilities;

TEST(MeshQualityTests, Basis)
{
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ -10, -10 }, Point2{ 10, 10 }});
   const auto p0 = points.addIfNew(Point2{ 0,0 });
   const auto p1 = points.addIfNew(Point2{ 1,0 });
   const auto p2 = points.addIfNew(Point2{ 0,std::sqrt(3.0) });
   const auto p3 = points.addIfNew(Point2{ 0,-1 });

   TrianglesNodes trianglesNodes;
   const auto c0 = trianglesNodes.addTriangle(p0, p1, p2);
   const auto c1 = trianglesNodes.addTriangle(p0, p1, p3);

   CellQuality2Fun* fun = CellQuality2::MinimumAngle;
   ASSERT_NEAR(MeshQuality::getQuality2(trianglesNodes, std::array<CellIndex, 1>{c0}, points, fun), 0.5, 1.0e-10);
   ASSERT_NEAR(MeshQuality::getQuality2(trianglesNodes, std::array<CellIndex, 1>{c1}, points, fun), 0.75, 1.0e-10);
   ASSERT_NEAR(MeshQuality::getQuality2(trianglesNodes, std::array<CellIndex, 2>{c0, c1}, points, fun), 0.5, 1.0e-10);
   ASSERT_NEAR(MeshQuality::getQuality2(trianglesNodes, std::array<CellIndex, 2>{c1, c0}, points, fun), 0.5, 1.0e-10);
   ASSERT_NEAR(MeshQuality::getQuality2WholeMesh(trianglesNodes, points, fun), 0.5, 1.0e-10);
}


TEST(MeshQualityTests, Fifteen)
{
   const double cos15 = (std::sqrt(2.0) + std::sqrt(6.0)) / 4;
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ -10, -10 }, Point2{ 10, 10 }});
   const auto p0 = points.addIfNew(Point2{ 0,0 });
   const auto p1 = points.addIfNew(Point2{ 1,0 });
   const auto p2 = points.addIfNew(Point2{ 0, std::sqrt(1/(cos15*cos15) - 1)});

   TrianglesNodes trianglesNodes;
   const auto c0 = trianglesNodes.addTriangle(p0, p1, p2);

   CellQuality2Fun* fun = CellQuality2::MinimumAngle;
   ASSERT_NEAR(MeshQuality::getQuality2(trianglesNodes, std::array<CellIndex, 1>{c0}, points, fun), 0.25, 1.0e-10);
   ASSERT_NEAR(MeshQuality::getQuality2WholeMesh(trianglesNodes, points, fun), 0.25, 1.0e-10);
}
