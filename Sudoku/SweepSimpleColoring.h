#pragma once

#include "FieldValue.h"
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
   FieldValue::Container collect(const FieldsInfo &) override;

   static std::vector<Link> getBilocationLinks(const CandidatesGrid &, FieldsInfo, Value);
   static FieldValue::Container collectSees2ColorsCandidates(const FieldsInfo &, Value, const ColorField &,
                                                             const FieldBits &);
   static bool removeSees2ColorsCandidates(CandidatesGrid &, const FieldsInfo &, Value, const ColorField &,
                                           const FieldBits &);
   static FieldValue::Container collectTwiceInAUnit(const FieldsInfo &, Value, const ColorField &);
   static bool removeTwiceInAUnit(CandidatesGrid &, Value, const ColorField &);
   static FieldValue::Container collectOneColorEmptiesHouse(const FieldsInfo &, Value, const ColorField &,
                                                            const FieldBits &);
   static bool removeOneColorEmptiesHouse(CandidatesGrid &, Value, const ColorField &, const FieldBits &);
   static bool sweep(CandidatesGrid &, Value);
};

} // namespace Sudoku
