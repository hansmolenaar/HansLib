#pragma once

#include "FieldValue.h"
#include "IStrategy.h"
#include "Link.h"

#include <vector>

namespace Sudoku
{
class CandidatesGrid;
class ColorField;
class FieldsInfo;

class SweepSimpleColoring : public IStrategy
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;

   static std::vector<Link> getBilocationLinks(const CandidatesGrid &, FieldsInfo, Value);
   static FieldValue::Container collectSees2ColorsCandidates(const FieldsInfo &, Value, const ColorField &,
                                                             const FieldBits &);
   static FieldValue::Container collectTwiceInAUnit(const FieldsInfo &, Value, const ColorField &);
   static FieldValue::Container collectOneColorEmptiesHouse(const FieldsInfo &, Value, const ColorField &,
                                                            const FieldBits &);
};

} // namespace Sudoku
