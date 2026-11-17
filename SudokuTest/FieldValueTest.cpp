#include <gtest/gtest.h>

#include "FieldValue.h"

using namespace Sudoku;

TEST(FieldValueTest, Hashing)
{
   const auto hasher = std::hash<Sudoku::FieldValue>();
   std::set<size_t> hashes;
   for (Field field : FieldAll)
   {
      for (Value value : ValueAll)
      {
         const FieldValue fv{field, value};
         const auto hash = hasher(fv);
         hashes.insert(hash);
      }
   }
   ASSERT_EQ(hashes.size(), NumField * NumValue);
}
