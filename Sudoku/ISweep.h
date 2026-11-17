#pragma once

#include "FieldValue.h"

#include <set>

namespace Sudoku
{
class CandidatesGrid;
class FieldsInfo;

class ISweep
{
 public:
   virtual ~ISweep() = default;

   virtual std::set<FieldValue> collect(const FieldsInfo &);
   virtual bool sweep(CandidatesGrid &);
   static bool iterate(ISweep &, CandidatesGrid &);
};

} // namespace Sudoku
