#pragma once

#include "ISweep.h"
#include <initializer_list>
#include <vector>

namespace Sudoku
{

class SweepCombined : public ISweep
{
 public:
   SweepCombined(std::initializer_list<ISweep *>);
   bool sweep(CandidatesGrid &) override;

 private:
   std::vector<ISweep *> m_sweepers;
};

} // namespace Sudoku
