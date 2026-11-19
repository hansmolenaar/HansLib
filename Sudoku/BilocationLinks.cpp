#include "BilocationLinks.h"

using namespace Sudoku;
using namespace Utilities;

BilocationLinks::BilocationLinks(const SudokuUtils::Value2Fields &value2fields, Value value) : m_value(value)
{
#if false
   for (const House &house : House::getAllHouses())
   {
      const auto &fields = value2fields[value];
      if (fields.size() == 2)
      {
         m_links.emplace_back(Cell::get(fields.front()), Cell::get(fields.back()), LinkType::Strong);
      }
   }

   UniqueSorted(m_links);
#endif
}
