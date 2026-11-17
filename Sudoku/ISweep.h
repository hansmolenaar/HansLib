#pragma once

#include "FieldValue.h"

namespace Sudoku
{
class CandidatesGrid;
class FieldsInfo;

class ISweep
{
 public:
   virtual ~ISweep() = default;

   virtual FieldValue::Container collect(const FieldsInfo &);
   virtual bool sweep(CandidatesGrid &);
   static bool iterate(ISweep &, CandidatesGrid &);
};

} // namespace Sudoku
