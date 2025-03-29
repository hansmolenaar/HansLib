#include <gtest/gtest.h>

#include "Permutation.h"
#include "Tetrahedron.h"
#include "TopologyDefines.h"

#include <algorithm>
#include <array>

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
