#include <gtest/gtest.h>

#include "Permutation.h"
#include "Tetrahedron.h"
#include "TetrahedronNodesOriented.h"
#include "TopologyDefines.h"
#include "Triangle.h"

#include <algorithm>
#include <array>
#include <numbers>

using namespace Topology;

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

