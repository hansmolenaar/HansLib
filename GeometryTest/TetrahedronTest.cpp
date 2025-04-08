#include <gtest/gtest.h>

#include "Permutation.h"
#include "PointClose.h"
#include "ReferenceShapeCube.h"
#include "Tetrahedron.h"
#include "TetrahedronNodesOriented.h"
#include "TopologyDefines.h"
#include "Triangle.h"

#include <algorithm>
#include <array>
#include <numbers>

using namespace Topology;

namespace
{
   void checkTranslationInvariance(const std::array<Point3, NumNodesOnTetrehadron>& org)
   {
      const PointClose<double, GeomDim3> areClose;
      std::array<Point3, NumNodesOnTetrehadron> shifted;
      str::transform(org, shifted.begin(), [](const Point3& p) {return p + Point3{ 1,2,3 }; });
      const double volumeOrg = Tetrahedron::getSignedVolume(org[0], org[1], org[2], org[3]);
      const double volumeShifted = Tetrahedron::getSignedVolume(shifted[0], shifted[1], shifted[2], shifted[3]);
      ASSERT_DOUBLE_EQ(volumeOrg, volumeShifted);

      const TetrahedronNodesOriented tetNodes{ 0,1,2,3 };
      const auto faceNodes = tetNodes.getFaces();
      for (const auto f : faceNodes)
      {
         const auto normalOrg = Triangle::getAreaDirected(org[f[0]], org[f[1]], org[f[2]]);
         const auto normalShifted = Triangle::getAreaDirected(shifted[f[0]], shifted[f[1]], shifted[f[2]]);
         ASSERT_TRUE(areClose.samePoints(normalOrg, normalShifted));
      }
   }
}
TEST(TetrahedronTest, Volume)
{
   const Point3 p0{ 0,0,0 };
   const Point3 p1{ 1,0,0 };
   const Point3 p2{ 0,1,0 };
   const Point3 p3{ 0,0,1 };
   const double volume = Tetrahedron::getSignedVolume(p0, p1, p2, p3);
   constexpr double expect = 1.0 / 6;
   ASSERT_DOUBLE_EQ(volume, expect);
}

TEST(TetrahedronTest, TranslationInvariance)
{
   checkTranslationInvariance(Tetrahedron::getRegularTetrahedron());
   checkTranslationInvariance(Tetrahedron::getTriRectangularTetrahedron(2, 3, 5));
}

TEST(TetrahedronTest, VolumePermuted)
{
   const Point3 p0{ 0,0,0 };
   const Point3 p1{ 1,0,0 };
   const Point3 p2{ 0,1,0 };
   const Point3 p3{ 0,0,1 };
   const std::array<Point3, NumNodesOnTetrehadron> points{ p0, p1, p2, p3 };
   std::vector<Permutation::Entry> p{ 0,1,2,3 };
   do
   {
      const Permutation permut = Permutation::Create(p);
      const auto parity = permut.getParity();

      auto tet = points;
      permut.apply(points.begin(), tet.begin());

      const double volume = Tetrahedron::getSignedVolume(tet[0], tet[1], tet[2], tet[3]);
      const double expect = parity == Permutation::Parity::EVEN ? 1.0 / 6 : -1.0 / 6;
      ASSERT_DOUBLE_EQ(volume, expect);
   } while (std::next_permutation(p.begin(), p.end()));
}

TEST(TetrahedronTest, RegularTetrahedron)
{
   const TetrahedronNodesOriented tetNodes{ 0,1,2,3 };
   const auto faceNodes = tetNodes.getFaces();
   const auto tet = Tetrahedron::getRegularTetrahedron();
   const double volume = Tetrahedron::getSignedVolume(tet[0], tet[1], tet[2], tet[3]);
   ASSERT_DOUBLE_EQ(volume, 8.0 / 3.0);
   const auto centerTet = (tet[0] + tet[1] + tet[2] + tet[3]) / Topology::NumNodesOnTetrehadron;

   for (const auto f : faceNodes)
   {
      const auto centerFace = Triangle::getCenter(tet[f[0]], tet[f[1]], tet[f[2]]);
      const auto normal = Triangle::getAreaDirected(tet[f[0]], tet[f[1]], tet[f[2]]);
      const double area = std::sqrt(PointUtils::GetNormSquared(normal));
      ASSERT_DOUBLE_EQ(area, 2 * std::sqrt(3.0));
      const Point3 dif = centerFace - centerTet;
      ASSERT_GT(PointUtils::innerProduct(normal, dif), 0.0);
   }
}

TEST(TetrahedronTest, SmallestDihedralAngle)
{
   const auto tet = Tetrahedron::getRegularTetrahedron();
   const double angle = Tetrahedron::getSmallestDihedralAngle(tet);
   ASSERT_DOUBLE_EQ(angle, std::acos(1.0 / 3.0));
}

TEST(TetrahedronTest, TriRectangularTetrahedron)
{
   const auto tet = Tetrahedron::getTriRectangularTetrahedron(2, 3, 5);
   const double volume = Tetrahedron::getSignedVolume(tet[0], tet[1], tet[2], tet[3]);
   ASSERT_DOUBLE_EQ(volume, 5.0);
}


TEST(TetrahedronTest, SmallDihedralAngle)
{
   for (size_t d = 0; d < GeomDim3; ++d)
   {
      std::array<double, GeomDim3> len{ 1,1,1 };
      len[d] = 0.001;
      const auto tet = Tetrahedron::getTriRectangularTetrahedron(len[0], len[1], len[2]);
      const double angle = Tetrahedron::getSmallestDihedralAngle(tet);
      ASSERT_LT(angle, 0.0015);
   }

}
TEST(TetrahedronTest, TriRectangularTetrahedronSmallestDihedralAngle)
{
   const auto tet = Tetrahedron::getTriRectangularTetrahedron();
   const double angle = Tetrahedron::getSmallestDihedralAngle(tet);
   ASSERT_DOUBLE_EQ(angle, std::acos(1 / std::sqrt(3.0)));
}

TEST(TetrahedronTest, CubeSplit)
{
   const std::array<Point3, NumNodesOnCube> vertices{
      Point3{ 0,0,0 }, Point3{ 1,0,0 }, Point3{ 0,1,0 }, Point3{ 1,1,0 },
      Point3{ 0,0,1 },  Point3{ 1,0,1 },  Point3{ 0,1,1 }, Point3{ 1,1,1 } };
   double volume = 0;
   for (const auto& tet : ReferenceShapeCube::getInstance().getStandardSplit())
   {
      volume += Tetrahedron::getSignedVolume(vertices[tet[0]], vertices[tet[1]], vertices[tet[2]], vertices[tet[3]]);
   }
   ASSERT_DOUBLE_EQ(volume, 1.0);
}
