#pragma once

namespace Sudoku
{
class CandidatesGrid;

class ISweep
{
 public:
   virtual ~ISweep() = default;
   virtual void sweep(CandidatesGrid &) = 0;
   static void iterate(ISweep &, CandidatesGrid &);
};

} // namespace Sudoku
