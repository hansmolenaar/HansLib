#include <gtest/gtest.h>

#include "Paraview.h"
#include "PointClose.h"
#include "ProjectToVtk.h"
#include "TriangleNodesOriented.h"
#include "TrianglesNodes.h"
#include "UniquePointCollectionBinning.h"

using namespace MeshGeneration;
using namespace Geometry;
using namespace Topology;

TEST(ProjectToVtkTest, SingleTriangle)
{
   const PointClose<GeomType, GeomDim2> areClose;
   const Simplex<GeomType, GeomDim2> triangle{ Point2{0,0}, Point2{1,0}, Point2{0,1} };
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>(triangle.begin(), triangle.end()));

   TrianglesNodes tnodes;
   tnodes.addTriangle(TriangleNodesOriented{ 0, 1, 2 });
   ProjectToVtk p2v("ProjectToVtkTest_SingleTriangle");
   p2v.addTrianglesAndBoundaries(tnodes, points, "mesh");

   const std::vector<const Vtk::VtkData*> vtkDatas = p2v.get();
   ASSERT_EQ(vtkDatas.size(), 2);
   ASSERT_EQ(vtkDatas.at(1)->getName().item, "mesh_PART_0_BDY_cycle_1");
}

TEST(ProjectToVtkTest, TwoTriangles)
{
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{{ -5, -5 }, { 5,5 }});
   const auto p0 = points.addIfNew({ -2, 0 });
   const auto p1 = points.addIfNew({ -1, 0 });
   const auto p2 = points.addIfNew({ -1, 1 });

   const auto p3 = points.addIfNew({ 1, 0 });
   const auto p4 = points.addIfNew({ 2, 0 });
   const auto p5 = points.addIfNew({ 1, 1 });
   TrianglesNodes tnodes;
   tnodes.addTriangle(TriangleNodesOriented{ p0, p1, p2 });
   tnodes.addTriangle(TriangleNodesOriented{ p3, p4, p5 });

   ProjectToVtk p2v("ProjectToVtkTest_TwoTriangles");
   p2v.addTrianglesAndBoundaries(tnodes, points, "mesh");

   const std::vector<const Vtk::VtkData*> vtkDatas = p2v.get();
   ASSERT_EQ(vtkDatas.size(), 4);
   ASSERT_EQ(vtkDatas.at(0)->getName().item, "mesh_PART_0");
   ASSERT_EQ(vtkDatas.at(1)->getName().item, "mesh_PART_0_BDY_cycle_1");
   ASSERT_EQ(vtkDatas.at(2)->getName().item, "mesh_PART_1");
   ASSERT_EQ(vtkDatas.at(3)->getName().item, "mesh_PART_1_BDY_cycle_1");
   //Paraview::WriteList(vtkDatas);
}

TEST(ProjectToVtkTest, TwoTouchingTriangles)
{
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{{ -5, -5 }, { 5,5 }});
   const auto p0 = points.addIfNew({ 0, 0 });
   const auto p1 = points.addIfNew({ -1, 0 });
   const auto p2 = points.addIfNew({ -1, 1 });

   const auto p3 = points.addIfNew({ 1, 0 });
   const auto p4 = points.addIfNew({ 1, 1 });
   TrianglesNodes tnodes;
   tnodes.addTriangle(TriangleNodesOriented{ p0, p1, p2 });
   tnodes.addTriangle(TriangleNodesOriented{ p3, p4, p0 });

   ProjectToVtk p2v("ProjectToVtkTest_TwoTouchingTriangles");
   p2v.addTrianglesAndBoundaries(tnodes, points, "mesh");

   const std::vector<const Vtk::VtkData*> vtkDatas = p2v.get();
   Paraview::WriteList(vtkDatas);
   ASSERT_EQ(vtkDatas.size(), 4);
   ASSERT_EQ(vtkDatas.at(0)->getName().item, "mesh_PART_0");
   ASSERT_EQ(vtkDatas.at(1)->getName().item, "mesh_PART_0_BDY_cycle_1");
   ASSERT_EQ(vtkDatas.at(2)->getName().item, "mesh_PART_1");
   ASSERT_EQ(vtkDatas.at(3)->getName().item, "mesh_PART_1_BDY_cycle_1");
   //Paraview::WriteList(vtkDatas);
}
