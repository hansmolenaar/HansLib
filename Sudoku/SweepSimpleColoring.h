#pragma once

#include "ISweep.h"
#include "Link.h"

namespace Sudoku
{
class CandidatesGrid;
class ColorField;
class FieldsInfo;

class SweepSimpleColoring : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
   static std::vector<Link> getBilocationLinks(const CandidatesGrid &, FieldsInfo, Value);
   static bool removeSees2ColorsCandidates(CandidatesGrid &, const FieldsInfo &, Value, const ColorField &,
                                           const FieldBits &);
   static bool removeTwiceInAUnit(CandidatesGrid &, Value, const ColorField &);
   static bool removeOneColorEmptiesHouse(CandidatesGrid &, Value, const ColorField &, const FieldBits &);
   static bool sweep(CandidatesGrid &, Value);
};

} // namespace Sudoku
