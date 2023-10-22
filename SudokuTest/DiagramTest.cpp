#include <gtest/gtest.h>

#include "Diagram.h"
#include "Potential.h"
#include "FieldInfoStatic.h"
#include "MyException.h"
#include "Defines.h"

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
   input[FieldInfoStatic::RowColToField(0, 0)] = 1;
   input[FieldInfoStatic::RowColToField(8, 8)] = 2;
   const Diagram diagram = Diagram::Create(input);

   std::ostringstream stream;
   stream << diagram;
   const std::string str = stream.str();
   ASSERT_FALSE(str.empty());
}

TEST(DiagramTest, GetPotentialsRow)
{
   constexpr RowColIndex RowToSkip = 6;
   constexpr Value ValueActive = RowToSkip + 1;
   constexpr RowColIndex ColToUse = 5;
   constexpr FieldIndex FieldToUse = FieldInfoStatic::RowColToField(RowToSkip, ColToUse);

   std::unordered_map<FieldIndex, Value> input;
   for (auto row : RowColAll)
   {
      if (row == RowToSkip) continue;
      input[FieldInfoStatic::RowColToField(row, ColToUse)] = row + 1;
   }

   const Diagram diagram = Diagram::Create(input);
   const auto potentials = diagram.getPotentials();
   ASSERT_EQ(potentials.at(FieldToUse).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldToUse).ContainsValue(ValueActive));
}


TEST(DiagramTest, GetPotentialsCol)
{
   constexpr RowColIndex ColToSkip = 8;
   constexpr Value ValueActive = ColToSkip + 1;
   constexpr RowColIndex RowToUse = 0;
   constexpr FieldIndex FieldToUse = FieldInfoStatic::RowColToField(RowToUse, ColToSkip);

   std::unordered_map<FieldIndex, Value> input;
   for (auto col : RowColAll)
   {
      if (col == ColToSkip) continue;
      input[FieldInfoStatic::RowColToField(RowToUse, col)] = col + 1;
   }

   const Diagram diagram = Diagram::Create(input);
   const auto potentials = diagram.getPotentials();
   ASSERT_EQ(potentials.at(FieldToUse).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldToUse).ContainsValue(ValueActive));
}


TEST(DiagramTest, GetPotentialsSubSquare)
{
   const std::array<FieldInfoStatic, NumFields>& instance = FieldInfoStatic::Instance();
   constexpr RowColIndex FieldToSkip = 79;

   std::unordered_map<FieldIndex, Value> input;
   Value value = 1;
   for (RowColIndex row = 6; row < NumRowCol; ++row)
   {
      for (RowColIndex col = 6; col < NumRowCol; ++col)
      {
         const FieldIndex field = FieldInfoStatic::RowColToField(row, col);
         if (field == FieldToSkip) continue;
         input[field] = value;
         ++value;
      }
   }

   const Diagram diagram = Diagram::Create(input);
   const std::string str = diagram.toString();
   ASSERT_FALSE(str.empty());

   const auto potentials = diagram.getPotentials();
   ASSERT_EQ(potentials.at(FieldToSkip).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldToSkip).ContainsValue(NumValues));
}


TEST(DiagramTest, OneStar1)
{
   std::array<Value, NumFields> diagramValues =
   {
      4, 9, 6,     0, 0, 2,    1, 8, 5,
      0, 7, 2,     1, 0, 0,    4, 9, 0,
      1, 0, 0,     4, 9, 0,    3, 7, 2,

      9, 4, 0,     6, 2, 7,    8, 5, 1,
      6, 2, 7,     0, 5, 1,    9, 4, 0,
      0, 5, 1,     0, 0, 3,    0, 0, 7,

      0, 0, 4,     0, 3, 9,    5, 1, 8,
      0, 0, 9,     5, 1, 8,    7, 0, 0,
      0, 0, 8,     7, 0, 0,    2, 0, 0,
   };

   const Diagram diagram = Diagram::Create(diagramValues);
   const std::string str = diagram.toString();
   ASSERT_FALSE(str.empty());

   const auto potentials = diagram.getPotentials();
   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(0, 3)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(0, 3)).ContainsValue(3));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(0, 4)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(0, 4)).ContainsValue(7));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(1, 8)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(1, 8)).ContainsValue(6));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(2, 1)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(2, 1)).ContainsValue(8));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(2, 2)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(2, 2)).ContainsValue(5));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(3, 2)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(3, 2)).ContainsValue(3));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(4, 3)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(4, 3)).ContainsValue(8));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(4, 8)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(4, 8)).ContainsValue(3));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(5, 0)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(5, 0)).ContainsValue(8));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(5, 6)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(5, 6)).ContainsValue(6));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(6, 1)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(6, 1)).ContainsValue(6));

   ASSERT_EQ(potentials.at(FieldInfoStatic::RowColToField(6, 3)).Count(), 1);
   ASSERT_TRUE(potentials.at(FieldInfoStatic::RowColToField(6, 3)).ContainsValue(2));

   const auto numSimple = str::count_if(potentials, [](const Potential& p) {return p.Count() == 1; });
   ASSERT_EQ(numSimple, 12);
}
