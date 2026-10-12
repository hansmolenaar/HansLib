#include <gtest/gtest.h>

#include "CandidatesGrid.h"

using namespace Sudoku;

TEST(CandidatesGridTest, DefaultConstructor)
{
   const CandidatesGrid cg;
   ASSERT_EQ(cg.getNumCandidates(), NumField * NumValue);
}

TEST(CandidatesGridTest, SetCandidates)
{
   constexpr Field field = 2;
   CandidatesGrid cg;
   cg.setCandidates(field, {3, 4});
   ASSERT_EQ(cg.getNumCandidates(), (NumField - 1) * NumValue + 2);
   cg.setCandidates(field, {4});
   ASSERT_EQ(cg.getNumCandidates(), (NumField - 1) * NumValue + 1);
   cg.setCandidates(field, {4});
   ASSERT_EQ(cg.getNumCandidates(), (NumField - 1) * NumValue + 1);

   ASSERT_ANY_THROW(cg.setCandidates(field, {3}));
   ASSERT_ANY_THROW(cg.setCandidates(field, {}));
}

TEST(CandidatesGridTest, RemoveCandidate)
{
   constexpr Field field = 2;
   CandidatesGrid cg;

   cg.setCandidates(field, {3, 4});
   ASSERT_EQ(cg[field].size(), 2);
   ASSERT_TRUE(cg[field].contains(3));
   cg.removeCandidate(field, 3);
   ASSERT_FALSE(cg[field].contains(3));
   ASSERT_TRUE(cg[field].contains(4));
   ASSERT_EQ(cg[field].size(), 1);

   ASSERT_EQ(cg.getNumCandidates(), (NumField - 1) * NumValue + 1);

   ASSERT_NO_THROW(cg.removeCandidate(field, 3));  // OK, not a candidate anymore
   ASSERT_ANY_THROW(cg.removeCandidate(field, 4)); // only candidate
}

TEST(CandidatesGridTest, ToOStream)
{
   CandidatesGrid cg;
   std::ostringstream stream;

   cg.setCandidates(9, {3, 4});
   cg.setCandidates(8, {8});
   cg.setCandidates(NumField - 1, {9});

   stream << cg;
   const auto retval = stream.str();
   // std::cout << retval;
   ASSERT_FALSE(retval.empty());
}
