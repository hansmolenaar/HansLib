#include "SolverSweepBiValueLinks.h"
#include "FieldInfoStatic.h"


using namespace Sudoku;


SolverSweepResult SolverSweepBiValueLinks::operator()(Potentials& potentials)
{
   return SolverSweepResult::NoChange;
}

ActiveFields SolverSweepBiValueLinks::GetBiValueFields(const Potentials& potentials)
{
   ActiveFields result;
   for (auto f : FieldInfoStatic::getAllFields())
   {
      if (potentials.get(f).count() == 2)
      {
         result.push_back(f);
      }
   }
   return result;
}