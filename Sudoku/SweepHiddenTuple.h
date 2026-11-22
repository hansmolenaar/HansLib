#pragma once

#include "House.h"
#include "ISweep.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class SweepHiddenTuple : public IStrategy
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;

   static FieldValue::Container collectHouse(const FieldsInfo &, const House &, Index);
   static FieldValue::Container collectTuplesOfSize(const FieldsInfo &, Index);
   // TODO static FieldValue::Container collect(const FieldsInfo &);

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
