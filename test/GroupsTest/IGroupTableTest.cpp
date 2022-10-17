#include <gtest/gtest.h>

#include "Groups/IGroupTable.h"
#include "IGroupTableTest.h"

namespace IGroupTableTest
{


   void TestInterface(const IGroupTable& table)
   {
      const int order = table.Order();
      ASSERT_TRUE(order > 0);
      const int id = table.Identity();
      ASSERT_TRUE(id >= 0 && id < order);

      for (int n = 0; n < order; ++n)
      {
         ASSERT_EQ(n, table(n, id));
         ASSERT_EQ(n, table(id, n));
         const int n_inv = table.Inverse(n);
         ASSERT_EQ(id, table(n, n_inv));
         ASSERT_EQ(id, table(n_inv, n));

         for (int m = 0; m < order; ++m)
         {
            int cmp = table(n, m);
            ASSERT_TRUE(cmp >= 0);
            ASSERT_TRUE(cmp < order);
         }
      }

      ASSERT_ANY_THROW(table.Inverse(order));
      ASSERT_ANY_THROW(table.Inverse(-1));

      ASSERT_ANY_THROW(table(id, order));
      ASSERT_ANY_THROW(table(order, id));


      ASSERT_ANY_THROW(table(id, -1));
      ASSERT_ANY_THROW(table(-1, id));
   }
};








