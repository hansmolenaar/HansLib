#include <gtest/gtest.h>

#include "MultiVariableRealValuedFunctionEvaluateCached.h"
#include "MultiVariableMonomial.h"
#include "Functors.h"

#include <array>

TEST(MultiVariableRealValuedFunctionEvaluateCachedTest, TestCaching)
{
   std::unique_ptr<IMultiVariableFunctionEvaluate> eval = std::make_unique<MultiVariableMonomial>(std::vector<int>{ 2, 1 });
   const MultiVariableRealValuedFunctionEvaluateCached  fie(std::move(eval));

   ASSERT_EQ(fie.getDimension(), 2);
   ASSERT_EQ(fie.getNumEvaluations(), 0);
   ASSERT_EQ(fie.getNumCacheHits(), 0);

   const std::array<double, 2> xy1{ 1,2 };
   const std::array<double, 2> xy2{ 2,1 };

   ASSERT_TRUE(Functors::AreClose()(fie(xy1), 2.0));
   ASSERT_EQ(fie.getNumEvaluations(), 1);
   ASSERT_EQ(fie.getNumCacheHits(), 0);

   ASSERT_TRUE(Functors::AreClose()(fie(xy1), 2.0));
   ASSERT_EQ(fie.getNumEvaluations(), 1);
   ASSERT_EQ(fie.getNumCacheHits(), 1);

   ASSERT_TRUE(Functors::AreClose()(fie(xy2), 4.0));
   ASSERT_EQ(fie.getNumEvaluations(), 2);
   ASSERT_EQ(fie.getNumCacheHits(), 1);

   ASSERT_TRUE(Functors::AreClose()(fie(xy2), 4.0));
   ASSERT_EQ(fie.getNumEvaluations(), 2);
   ASSERT_EQ(fie.getNumCacheHits(), 2);
}
