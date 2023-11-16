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

   class SweepAllTypes : public ISolverSweep
   {
   public:
      SweepAllTypes(ISubSetPotentialsSweep& sweep);
      SolverSweepResult operator()(Potentials& potentials) override;
   private:
      ISubSetPotentialsSweep& m_sweep;
   };

   SweepAllTypes::SweepAllTypes(ISubSetPotentialsSweep& sweep) : m_sweep(sweep)
   {}


   SolverSweepResult SweepAllTypes::operator()(Potentials& potentials)
   {
      bool anyChange = false;
      for (auto type : SubSetTypeAll)
      {
         if (SweepItems(type, potentials, m_sweep))
         {
            anyChange = true;
         }
      }

      if (potentials.isSolved()) return SolverSweepResult::Solved;
      return anyChange ? SolverSweepResult::Change : SolverSweepResult::NoChange;
   }
}

SolverSweepResult SolverSweepTrivial::operator()(Potentials& potentials)
{
   SubSetPotentialsSweepSingles sweepSingles;
   SweepAllTypes sweep(sweepSingles);
   return sweep(potentials);
}


SolverSweepResult SweepAllClusters::operator()(Potentials& potentials)
{
   SubSetPotentialsSweepPairs sweepClusters;
   SweepAllTypes sweep(sweepClusters);
   return sweep(potentials);
}
