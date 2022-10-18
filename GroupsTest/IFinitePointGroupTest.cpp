#include <gtest/gtest.h>

#include "Groups/IGroupTable.h"
#include "IFinitePointGroupTest.h"
#include "Groups/IFinitePointGroup.h"
#include "Groups/IFinitePointGroupAction.h"

#include <span>


void IFinitePointGroupTest::TestPointGroup(const IFinitePointGroup& group, std::span<const double> point)
{
   const int order = group.GroupTable().Order();
   const int geomdim = point.size();
   for (int n = 0; n < order; ++n)
   {
      std::vector<double> opern(geomdim);
      group.GetElement(n).Transform(point, opern);
      for (int m = 0; m < order; ++m)
      {
         std::vector<double> operm(geomdim);
         group.GetElement(m).Transform(opern, operm);

         const int expect = group.GroupTable()(n, m);
         std::vector<double> pointExpect(geomdim);
         group.GetElement(expect).Transform(point, pointExpect);

         for (int d = 0; d < point.size(); ++d)
         {
            ASSERT_NEAR(operm[d], pointExpect[d], 1.0e-12);
         }
      }
   }
}









