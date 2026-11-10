#pragma once

#include "ISweep.h"
#include "Link.h"

namespace Sudoku
{
class CandidatesGrid;

class SweepSimpleColoring : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
   static std::vector<Link> getBilocationLinks(const CandidatesGrid &, Value);
};

} // namespace Sudoku
