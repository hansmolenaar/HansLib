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

   virtual bool sweep(CandidatesGrid &);
   static bool iterate(ISweep &, CandidatesGrid &);
};

} // namespace Sudoku
