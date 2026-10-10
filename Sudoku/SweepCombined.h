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
   void sweep(Grid &) override;

 private:
   std::vector<ISweep *> m_sweepers;
};

} // namespace Sudoku
