#pragma once

#include "SolverSweep.h"

#include <unordered_map>

namespace Sudoku
{

   using ActiveFields = boost::container::static_vector<FieldIndex, NumFields>;
   using Color = FieldIndex;
   
   struct FieldColor
   {
      FieldIndex Field;
      Color  Color;
   };
   using ColorInComponent = boost::container::static_vector<FieldColor, NumFields>;
   using ColorInAllComponents = boost::container::static_vector<ColorInComponent, NumFields>;

   class SolverSweepBiValueLinksSingleValue : public ISolverSweep
   {
   public:
      explicit SolverSweepBiValueLinksSingleValue(Value value);
      SolverSweepResult operator()(Potentials& potentials) override;
      static ActiveFields GetBiValueFields(const Potentials& potentials, Value value);
      static std::vector<std::pair<FieldIndex, FieldIndex>> GetBiValueAdjecencies(const Potentials& potentials, Value value);
      static ColorInAllComponents GetColoring(const Potentials& potentials, Value value);
   private:
      Value getOtherValue(const Potentials& potentials, FieldIndex field) const;
      Value m_value;
   };

   class SolverSweepBiValueLinks : public ISolverSweep
   {
   public:
      SolverSweepResult operator()(Potentials& potentials) override;

   private:
   };


}