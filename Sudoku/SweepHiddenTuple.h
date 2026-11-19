#pragma once

#include "House.h"
#include "ISweep.h"
#include "SudokuDefines.h"

namespace Sudoku
{

namespace SweepHiddenTuple
{
bool sweepHouse(CandidatesGrid &, const House &, Index);
bool sweep(CandidatesGrid &, Index);
FieldValue::Container collectHouse(const FieldsInfo &, const House &, Index);
FieldValue::Container collectTuplesOfSize(const FieldsInfo &, Index);
FieldValue::Container collect(const FieldsInfo &);

}; // namespace SweepHiddenTuple

class SweepHiddenSingle : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

class SweepHiddenPair : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

class SweepHiddenTriplet : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

class SweepHiddenQuartet : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

} // namespace Sudoku
