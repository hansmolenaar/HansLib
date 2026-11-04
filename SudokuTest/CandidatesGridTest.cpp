#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include <iostream>

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

TEST(CandidatesGridTest, MesaageText)
{
   constexpr Field field = 2;
   constexpr Value value = 3;
   CandidatesGrid cg;
   cg.setGiven(field, value);
   const bool anyChange = cg.removeCandidate(field, value + 1);
   ASSERT_FALSE(anyChange);
   ASSERT_ANY_THROW(cg.removeCandidate(field, value));
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

TEST(CandidatesGridTest, Symmetries)
{
   const std::string str = "020010840709200001604000290000030020800102004090040000037000602100005903062070050";
   CandidatesGrid grid = CandidatesGrid::FromString(str, '0');
   grid.update();

   const std::vector<CandidatesGrid> grids = CandidatesGrid::getSymmetricEquivalents(grid);
   // std::cout << grids[0] << "\n";
   // std::cout << grids[7] << "\n";

   const auto expect = grid.getNumCandidates();
   for (const auto &tg : grids)
   {
      ASSERT_EQ(tg.getNumCandidates(), expect);
      tg.checkValidSudoku();
   }
}
