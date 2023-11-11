#include "SolverSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"
#include "SubSetPotentialsSweep.h"

#include<boost/container/static_vector.hpp>

using namespace Sudoku;

namespace
{
   bool SweepItems(SubSetType type, Potentials& potentials)
   {
      bool anyChange = false;
      SubSetPotentialsSweepSingles sweep;
      for (auto index : SubSetsAll)
      {
         auto subSetPotentials = potentials.getSubSetPotentials(type, index);
         if (sweep(subSetPotentials))
         {
            anyChange = true;
         }
      }
      return anyChange;
   }
}

bool SolverSweepRow::operator()(Potentials& potentials)
{
   return SweepItems(SubSetType::Row, potentials);
}

bool SolverSweepColumns::operator()(Potentials& potentials)
{
   return SweepItems(SubSetType::Column, potentials);
}

bool SolverSweepSubSquares::operator()(Potentials& potentials)
{
   return SweepItems(SubSetType::SubSquare, potentials);
}

bool SolverSweepTrivial::operator()(Potentials& potentials)
{
   const bool changedRow = SolverSweepRow()(potentials);
   const bool changedColumns = SolverSweepColumns()(potentials);
   const bool changedSubSquares = SolverSweepSubSquares()(potentials);
   return changedRow || changedColumns || changedSubSquares;
}