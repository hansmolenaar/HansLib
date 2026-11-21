#pragma once

#include "GridChange.h"
#include <initializer_list>
#include <span>
#include <vector>

namespace Sudoku
{
class IStrategy;
class CandidatesGrid;
class FieldsInfo;

class Sweep
{
 public:
   Sweep(std::initializer_list<IStrategy *>);
   Sweep(std::span<IStrategy *>);

   GridChange singleSweep(CandidatesGrid &, FieldsInfo &) const;
   bool singleSweepAndUpdate(CandidatesGrid &, FieldsInfo &) const;
   bool iterate(CandidatesGrid &, FieldsInfo &);

 private:
   std::vector<IStrategy *> m_strategies;
};

} // namespace Sudoku
