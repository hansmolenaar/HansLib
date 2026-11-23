#pragma once

#include "GridChange.h"
#include "Strategies.h"

namespace Sudoku
{
class IStrategy;
class CandidatesGrid;
class FieldsInfo;

class Sweep
{
 public:
   explicit Sweep(IStrategy *);
   explicit Sweep(Strategies &&);

   GridChange singleSweep(CandidatesGrid &, FieldsInfo &) const;
   bool singleSweepAndUpdate(CandidatesGrid &, FieldsInfo &) const;
   bool iterate(CandidatesGrid &, FieldsInfo &);

 private:
   Strategies m_strategies;
};

} // namespace Sudoku
