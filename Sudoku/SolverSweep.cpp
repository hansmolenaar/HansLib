#include "SolverSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"

#include<boost/container/static_vector.hpp>

using namespace Sudoku;

template<typename GetFields>
bool SweepItems(GetFields getFields, Potentials& potentials)
{
   bool anyChange = false;
   for (int item = 0; item < 9; ++item)
   {
      boost::container::static_vector<Value, NumRowCol> unsetMe;

      // Collect values that are unique
      for (auto field : getFields(item))
      {
         const auto val = potentials.GetSingleOrUndefined(field);
         if (val != ValueUndefined) unsetMe.push_back(val);
      }

      if (unsetMe.empty()) continue;

      for (auto field : getFields(item))
      {
         if (potentials.isSingle(field)) continue;
         for (auto value : unsetMe)
         {
            if (potentials.unset(field, value))
            {
               anyChange = true;
            }
         }
      }
   }
   return anyChange;
}

bool SolverSweepRow::operator()(Potentials& potentials)
{
   return SweepItems(FieldInfoStatic::GetRow, potentials);
}

bool SolverSweepColumns::operator()(Potentials& potentials)
{
   return SweepItems(FieldInfoStatic::GetCol, potentials);
}

bool SolverSweepSubSquares::operator()(Potentials& potentials)
{
   return SweepItems(FieldInfoStatic::GetSubSquare, potentials);
}

bool SolverSweepTrivial::operator()(Potentials& potentials)
{
   const bool changedRow = SolverSweepRow()(potentials);
   const bool changedColumns = SolverSweepColumns()(potentials);
   const bool changedSubSquares = SolverSweepSubSquares()(potentials);
   return changedRow || changedColumns || changedSubSquares;
}