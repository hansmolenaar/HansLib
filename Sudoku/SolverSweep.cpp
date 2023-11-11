#include "SolverSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"
#include "SubSetPotentialsSweep.h"

#include<boost/container/static_vector.hpp>

using namespace Sudoku;

namespace
{
   bool SweepItems(SubSetType type, Potentials& potentials, ISubSetPotentialsSweep& sweep)
   {
      bool anyChange = false;
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


   bool SweepAllTypes(Potentials& potentials, ISubSetPotentialsSweep& sweep)
   {
      bool anyChange = false;
      for (auto type : SubSetTypeAll)
      {
         if (SweepItems(type, potentials, sweep))
         {
            anyChange = true;
         }
      }
      return anyChange;
   }
}

bool SolverSweepTrivial::operator()(Potentials& potentials)
{
   SubSetPotentialsSweepSingles sweep;
   return SweepAllTypes(potentials, sweep);
}


bool SolverSweepPairs::operator()(Potentials& potentials)
{
   SubSetPotentialsSweepPairs sweep;
   return SweepAllTypes(potentials, sweep);
}
