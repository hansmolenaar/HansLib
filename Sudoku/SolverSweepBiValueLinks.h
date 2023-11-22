#pragma once

#include "SolverSweep.h"

namespace Sudoku
{

   using ActiveFields = boost::container::static_vector<FieldIndex, NumFields>;

   class SolverSweepBiValueLinks : public ISolverSweep
   {
   public:
      SolverSweepResult operator()(Potentials& potentials) override;
      static ActiveFields GetBiValueFields(const Potentials& potentials);
   private:
   };


}