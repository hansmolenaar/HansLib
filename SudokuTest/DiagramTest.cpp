#include <gtest/gtest.h>

#include "Diagram.h"
#include "Potential.h"
#include "FieldInfoStatic.h"
#include "MyException.h"

using namespace Sudoku;

TEST(DiagramTest, DuplicateRow)
{
   std::unordered_map<FieldIndex, Value> input;
   input[FieldInfoStatic::RowColToField(2, 2)] = 1;
   input[FieldInfoStatic::RowColToField(2, 4)] = 1;

   ASSERT_THROW(Diagram::Create(input), MyException);
}

TEST(DiagramTest, DuplicateCol)
{
   std::unordered_map<FieldIndex, Value> input;
   input[FieldInfoStatic::RowColToField(2, 2)] = 1;
   input[FieldInfoStatic::RowColToField(4, 2)] = 1;

   ASSERT_THROW(Diagram::Create(input), MyException);
}

TEST(DiagramTest, DuplicateSubSquare)
{
   std::unordered_map<FieldIndex, Value> input;
   input[FieldInfoStatic::RowColToField(4, 4)] = 1;
   input[FieldInfoStatic::RowColToField(5, 5)] = 1;

   ASSERT_THROW(Diagram::Create(input), MyException);
}
