#include <gtest/gtest.h>

#include "Diagram.h"
#include "Potential.h"
#include "FieldInfoStatic.h"
#include "MyException.h"
#include "Defines.h"
#include "Solver.h"
#include "SolverSweep.h"
#include "TestModels.h"

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
   const Diagram diagram = TestModels::getOneStar1();
   ASSERT_FALSE(diagram.isSolved());
   const std::string str = diagram.toString();
   ASSERT_FALSE(str.empty());

   auto potentials = diagram.getPotentials();
   SubSetPotentialsSweepSingles sweepSingles;
   SolverSweepSubSetTypeAll sweep(sweepSingles);
   sweep(potentials);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(0, 3)), 3);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(0, 4)), 7);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(1, 8)), 6);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(2, 1)), 8);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(2, 2)), 5);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(3, 2)), 3);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(4, 3)), 8);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(4, 8)), 3);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(5, 0)), 8);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(5, 6)), 6);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(6, 1)), 6);

   ASSERT_EQ(potentials.getSingleOrUndefined(FieldInfoStatic::RowColToField(6, 3)), 2);

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
