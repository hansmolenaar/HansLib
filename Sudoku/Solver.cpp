#include "Potential.h"
#include "Defines.h"
#include "FieldInfoStatic.h"
#include "Solver.h"

using namespace Sudoku;

class ISolverSweep
{
public:
   virtual std::vector<FieldValue> Action(const std::array<Potential, NumFields> state) = 0;
};

class SolverSweepSingletons : public ISolverSweep
{
public:
   std::vector<FieldValue> Action(const std::array<Potential, NumFields> state) override;
};

std::vector<FieldValue> SolverSweepSingletons::Action(const std::array<Potential, NumFields> state)
{
   std::vector<FieldValue> result;
   const auto& infoAll = FieldInfoStatic::Instance();
   for (const auto& info : infoAll)
   {
      if (state.at(info.Field).Count() == 1)
      {
         const auto& found = state.at(info.Field).getSingleValue();
         result.emplace_back(info.Field, found);
      }
   }
   return result;
}

Solver::Solver(Diagram diagramIn, Diagram state) :
   m_diagramIn(std::move(diagramIn)),
   m_diagramState(std::move(state)),
   m_isSolved(m_diagramState.isSolved())
{
}

const Diagram& Solver::getState() const
{
   return m_diagramState;
}

bool Solver::isSolved() const
{
   return m_isSolved;
}

Solver Solver::Create(const Diagram& diagramIn)
{
   Diagram diagram = diagramIn;
   while (!diagram.isSolved())
   {
      SolverSweepSingletons sweep;
      const auto& updates = sweep.Action(diagram.getPotentials());
      if (updates.empty()) break;
     
      std::array<Value, NumFields> values = diagram.getState();
      Potentials potentials = diagram.getPotentialS();
      for (const auto fv : updates)
      {
         if (fv.Value != ValueUndefined)
         {
            potentials.SetSingle(fv.Field, fv.Value);
         }
     }
      diagram = Diagram::Create(potentials);
   }

   return Solver(diagramIn,diagram);
}