#pragma once

#include "ISweep.h"
#include <initializer_list>
#include <span>
#include <vector>

namespace Sudoku
{

class SweepCombined : public ISweep
{
 public:
   SweepCombined(std::initializer_list<ISweep *>);
   SweepCombined(std::span<ISweep *>);
   bool sweep(CandidatesGrid &) override;
   static SweepCombined *getSimpleSweepers();

 private:
   std::vector<ISweep *> m_sweepers;
};

} // namespace Sudoku
