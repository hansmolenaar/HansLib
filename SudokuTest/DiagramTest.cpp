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

TEST(DiagramTest, ValuesWrong)
{
   std::unordered_map<FieldIndex, Value> input;
   input[FieldInfoStatic::RowColToField(2, 2)] = ValueUndefined - 1;
   ASSERT_THROW(Diagram::Create(input), MyException);

   input[FieldInfoStatic::RowColToField(2, 2)] = NumFields;
   ASSERT_THROW(Diagram::Create(input), MyException);
}


TEST(DiagramTest, Operator)
{
   std::unordered_map<FieldIndex, Value> input;
   const FieldIndex field = FieldInfoStatic::RowColToField(2, 3);
   input[field] = 1;
   const Diagram diagram = Diagram::Create(input);
   ASSERT_EQ(diagram(field), 1);
   ASSERT_EQ(diagram(field + 1), ValueUndefined);
}


TEST(DiagramTest, Stream)
{
   std::unordered_map<FieldIndex, Value> input;
   input[FieldInfoStatic::RowColToField(0,0)] = 1;
   input[FieldInfoStatic::RowColToField(8, 8)] = 2;
   const Diagram diagram = Diagram::Create(input);

   std::ostringstream stream;
   stream << diagram;
   const std::string str = stream.str();
   ASSERT_FALSE(str.empty());
}
