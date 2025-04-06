#include <gtest/gtest.h>

#include "CellQuality3.h"
#include "Tetrahedron.h"

using namespace Geometry;

TEST(CellQuality3Test, Regualar)
{
   const auto tet = Tetrahedron::getRegularTetrahedron();
   const double quality = CellQuality3::SmallestDihedralAngle(tet);
   ASSERT_DOUBLE_EQ(quality, 1.0);
}

