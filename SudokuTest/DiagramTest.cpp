#include <gtest/gtest.h>

#include "Diagram.h"
#include "Potential.h"
#include "FieldInfoStatic.h"
#include "MyException.h"
#include "Defines.h"
#include "Solver.h"
#include "SolverSweep.h"

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
   constexpr FieldIndex field = FieldInfoStatic::RowColToField(2, 3);
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

TEST(DiagramTest, Simple)
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
   ASSERT_FALSE(diagram.isSolved());
   const std::string str = diagram.toString();
   ASSERT_FALSE(str.empty());

   auto potentials = diagram.getPotentials();
   SolverSweepTrivial sweep;
   sweep(potentials);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(0, 3)), 3);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(0, 4)), 7);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(1, 8)), 6);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(2, 1)), 8);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(2, 2)), 5);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(3, 2)), 3);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(4, 3)), 8);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(4, 8)), 3);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(5, 0)), 8);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(5, 6)), 6);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(6, 1)), 6);

   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldInfoStatic::RowColToField(6, 3)), 2);

   int numSimple = 0;
   for (FieldIndex field = 0; field < NumFields; ++field)
   {
      if (potentials.isSingle(field)) numSimple += 1;
   }
   ASSERT_EQ(numSimple, 65);
}

TEST(DiagramTest, IsNotCorrectEmpty)
{
   const std::array<FieldInfoStatic, NumFields>& instance = FieldInfoStatic::Instance();
   std::unordered_map<FieldIndex, Value> input;

   const Diagram diagram = Diagram::Create(input);
   ASSERT_FALSE(diagram.isSolved());
}
