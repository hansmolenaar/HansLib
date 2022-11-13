#include <gtest/gtest.h>

#include "Permutaion/Permutation.h" 

TEST(PermutaionTest, Trivial)
{
   const Permutation permut = Permutation::CreateTrivial(2);
   ASSERT_EQ(permut(0), 0);
   ASSERT_EQ(permut(1), 1);
   ASSERT_ANY_THROW(permut(-1));
   ASSERT_ANY_THROW(permut(2));
   ASSERT_ANY_THROW(Permutation::CreateTrivial(0));
}


