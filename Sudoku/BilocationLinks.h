#pragma once

#include "Link.h"
#include "SudokuUtils.h"

namespace Sudoku
{

class CandidatesGrid;

class BilocationLinks
{
 public:
   BilocationLinks(const SudokuUtils::Value2Fields &, Value);

 private:
   std::vector<Link> m_links;
   Value m_value;
};

} // namespace Sudoku
