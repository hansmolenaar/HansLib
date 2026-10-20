#pragma once

namespace Sudoku
{
class CandidatesGrid;

class ISweep
{
 public:
   virtual ~ISweep() = default;
   virtual bool sweep(CandidatesGrid &) = 0;
   static bool iterate(ISweep &, CandidatesGrid &);
};

} // namespace Sudoku
