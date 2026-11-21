#pragma once

#include "IStrategy.h"

namespace Sudoku
{
class CandidatesGrid;
class FieldsInfo;

class ISweep : public IStrategy
{
 public:
   virtual ~ISweep() = default;

   virtual bool sweep(CandidatesGrid &, FieldsInfo &);
   static bool iterate(ISweep &, CandidatesGrid &, FieldsInfo &);
};

} // namespace Sudoku
