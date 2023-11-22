#pragma once

#include "SolverSweep.h"

#include <unordered_map>

namespace Sudoku
{

   using ActiveFields = boost::container::static_vector<FieldIndex, NumFields>;

   class SolverSweepBiValueLinks : public ISolverSweep
   {
   public:
      SolverSweepResult operator()(Potentials& potentials) override;
      static ActiveFields GetBiValueFields(const Potentials& potentials, Value value);
      static std::vector<std::pair<FieldIndex,FieldIndex>> GetBiValueAdjecencies(const Potentials& potentials, Value value);
      static std::unordered_multimap<int, FieldIndex> GetColoredNodes(const Potentials& potentials, Value value);
   private:
   };


}