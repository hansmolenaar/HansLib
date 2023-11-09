#include "SolverSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"

#include<boost/container/static_vector.hpp>

using namespace Sudoku;

bool SolverSweepRow::operator()(Potentials& potentials)
{
   bool anyChange = false;
   for (auto row : RowColAll)
   {
      boost::container::static_vector<Value, NumRowCol> unsetMe;
      // Collect values that are unique
      for (auto field : FieldInfoStatic::GetRow(row))
      {
         const auto val = potentials.GetSingleOrUndefined(field);
         if (val != ValueUndefined) unsetMe.push_back(val);
      }

      if (unsetMe.empty()) continue;

      for (auto field : FieldInfoStatic::GetRow(row))
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
