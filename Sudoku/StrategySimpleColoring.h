#pragma once

#include "FieldValue.h"
#include "IStrategy.h"
#include "Link.h"

#include <vector>

namespace Sudoku
{
class CandidatesGrid;
class Network;
class ColorField;
class FieldsInfo;

class StrategySimpleColoring : public IStrategy
{
 public:
   void collect(const FieldsInfo &, FieldValues &) override;
   std::string getName() const override;

   static std::vector<Link> getBilocationLinks(const CandidatesGrid &, FieldsInfo, Value);
   static void collectSees2ColorsCandidates(const FieldsInfo &, Value, const Network &, const ColorField &,
                                            const FieldBits &, FieldValues &);
   static void collectTwiceInAUnit(const FieldsInfo &, Value, const Network &, const ColorField &, FieldValues &);
   static void collectOneColorEmptiesHouse(const FieldsInfo &, Value, const Network &, const ColorField &,
                                           const FieldBits &, FieldValues &);
};

} // namespace Sudoku
