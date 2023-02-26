#include <gtest/gtest.h>

#include "CompassSearch.h" 
#include "MultiVariableFunctionExamples.h"
#include "Functors.h"
#include "Single.h"

TEST(CompassSearchTest, SingleArgument)
{
   std::unique_ptr<IMultiVariableFunctionEvaluate> fie = MultiVariableFunctionExamples::GetPolynomial(std::vector< std::pair<std::vector<int>, double>>{ {std::vector<int>{2}, 1.0}});
   CompassSearch cs(std::move(fie), std::vector<double>{0.9}, 1.0);
   ASSERT_EQ(cs.getNumIterations(), 0);
   bool succes = cs.iterate();
   ASSERT_TRUE(succes);
   ASSERT_EQ(cs.getNumIterations(), 1);
   constexpr Functors::AreClose areClose;
   ASSERT_TRUE(areClose(Utilities::Single(cs.getArgMin()), -0.1));
   ASSERT_TRUE(areClose(cs.getEvalMin(), 0.01));
}
