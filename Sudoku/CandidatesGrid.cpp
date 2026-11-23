#include "CandidatesGrid.h"
#include "BitsetUtils.h"
#include "Cell.h"
#include "Contains.h"
#include "FieldsInfo.h"
#include "House.h"
#include "MyAssert.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "Sweep.h"
#include "SweepNakedSingle.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{

typedef std::array<std::string, NumBoxIndex> (*candidatesToStringsPtr)(const Candidates &);

std::vector<std::string> ToStrings(const CandidatesGrid &grid, candidatesToStringsPtr fun)
{
   const FieldsInfo finfo(grid);
   std::vector<std::string> result;
   const std::string hsep = "*-----------------*-----------------*-----------------*";
   const std::string box_hsep_top_bot = "*******************************************************";
   const std::string box_hsep_x = box_hsep_top_bot; //"*             *             *             *";
   const std::string vsep = " | ";
   const std::string box_vsep = "*";

   for (const auto row : IndexAll)
   {
      if (row == 0)
      {
         result.push_back(box_hsep_top_bot);
      }
      else if (row % 3 == 0)
      {
         result.push_back(box_hsep_x);
      }
      else
      {
         result.push_back(hsep);
      }
      const size_t pos = result.size();
      result.push_back({}); // Open 3 new lines
      result.push_back({});
      result.push_back({});

      const House house{HouseType::Row, row};
      const auto houseFields = finfo.getHouseFields(house);
      for (Field field : SudokuUtils::FieldBitsToRange(houseFields))
      {
         const auto cell = Cell::get(field);
         const auto cstrings = fun(grid[field]);

         for (auto p : BoxIndexAll)
         {
            if (cell.col == 0)
            {
               result.at(pos + p).append(box_vsep + " ");
            }
            else if ((cell.col) % 3 == 0)
            {
               result.at(pos + p).append(" " + box_vsep + " ");
            }
            else
            {
               result.at(pos + p).append(vsep);
            }
            result.at(pos + p).append(cstrings.at(p));
         }
      }
      for (size_t p = pos; p < pos + NumBoxIndex; ++p)
      {
         result.at(p).append(" " + box_vsep);
      }
   }

   result.push_back(box_hsep_top_bot);
   return result;
}

typedef const Cell &(*TransformCell)(const Cell &);
CandidatesGrid Transform(const CandidatesGrid &grid, TransformCell transformCell)
{
   CandidatesGrid result;
   for (const auto &cell : Cell::getAll())
   {
      const auto &tcell = transformCell(cell);
      for (Value value : ValueAll)
      {
         if (!grid[cell.field].contains(value))
         {
            result.removeCandidate(tcell.field, value);
         }
      }
   }
   return result;
}

} // namespace

Index CandidatesGrid::getNumCandidates() const
{
   return std::accumulate(m_candidates.begin(), m_candidates.end(), static_cast<Index>(0),
                          [](Index acc, const Candidates &c) { return acc + c.size(); });
}

bool CandidatesGrid::removeCandidates(Field field, ValueBits valueBits)
{
   GridChange change;
   for (Value value : SudokuUtils::ValueBitsToRange(valueBits))
   {
      change += m_candidates.at(field).removeCandidate(value);
   }
   return change.numFieldsSolved > 0;
}

bool CandidatesGrid::setCandidates(Field field, std::initializer_list<Value> values)
{
   return m_candidates.at(field).setOnly(values);
}

bool CandidatesGrid::setCandidates(Index row, Index col, std::initializer_list<Value> values)
{
   return setCandidates(Cell::rowCol2Field(row, col), values);
}

bool CandidatesGrid::removeCandidate(Field field, Value value)
{
   return m_candidates.at(field).removeCandidate(value).numValuesChanged > 0;
}

GridChange CandidatesGrid::apply(const FieldValue::Container &fieldValues, FieldsInfo &finfo)
{
   GridChange change;
   for (const auto &fv : fieldValues)
   {
      change += m_candidates.at(fv.field).removeCandidate(fv.value);
   }
   if (change.numValuesChanged > 0)
   {
      finfo.update();
   }
   return change;
}

bool CandidatesGrid::applyAndUpdate(const FieldValue::Container &fieldValues, FieldsInfo &finfo)
{
   const auto changes = apply(fieldValues, finfo);
   if (changes.numValuesChanged == 0)
   {
      return false;
   }
   if (changes.numFieldsSolved > 0)
   {
      update(finfo);
   }
   return true;
}

void CandidatesGrid::setGiven(Field field, Value value)
{
   MyAssert((*this)[field].size() == NumValue);
   setCandidates(field, {value});
}

void CandidatesGrid::setGiven(Index row, Index col, Value value)
{
   const Field field = Cell::getFromRowCol(row, col).field;
   setGiven(field, value);
}

void CandidatesGrid::update(FieldsInfo &finfo)
{
   SweepNakedSingle strategy;
   Sweep(&strategy).iterate(*this, finfo);
}

void CandidatesGrid::checkValidSudoku() const
{
   const FieldsInfo finfo(*this);
   // Check if value within bounds
   constexpr Value lwr = ValueAll.front();
   constexpr Value upr = ValueAll.back();
   for (const auto f : FieldAll)
   {
      const auto value = (*this)[f].tryGetSolvedValue();
      if (value.has_value())
      {
         MyAssert(*value >= lwr && *value <= upr, "checkValidSudoku invalid value " + std::to_string(*value));
      }
   }

   // Check rule of 1
   for (const auto &house : House::getAllHouses())
   {
      ValueBits isUsed;
      ValueBits isSolved;
      for (Field field : FieldAll | std::views::filter(Bitset{finfo.getHouseFields(house)}))
      {
         const auto value = (*this)[field].tryGetSolvedValue();
         if (value.has_value())
         {
            // MyAssert(!isUsed.test(value.value()));
            MyAssert(!isSolved.test(value.value()), "checkValidSudoku solved value " + std::to_string(*value) +
                                                        " specified at least twice in house " + House::toString(house));
            isUsed.set(value.value());
            isSolved.set(value.value());
         }
         else
         {
            isUsed |= (*this)[field].getValues();
         }
      }
      MyAssert(isUsed.count() == NumValue, "checkValidSudoku candidate missing in house");
   }
}

CandidatesGrid CandidatesGrid::FromString(const std::string &str, char valueUndef)
{
   MyAssert(str.length() == NumField, "CandidatesGrid::FromString expected string of length " +
                                          std::to_string(NumField) + ", actual " + std::to_string(str.length()));
   CandidatesGrid result;
   for (Field f : FieldAll)
   {
      const char c = str[f];
      if (c != valueUndef)
      {
         const Value value = c - '0';
         MyAssert(Utilities::Contains(ValueAll, value));
         result.setGiven(f, value);
      }
   }
   result.checkValidSudoku();
   return result;
}

CandidatesGrid CandidatesGrid::FromValues(const std::array<Value, NumField> &values)

{
   CandidatesGrid result;
   for (Field f : FieldAll)
   {
      if (values[f] != 0)
      {
         result.setGiven(f, values[f]);
      }
   }
   result.checkValidSudoku();
   return result;
}

bool CandidatesGrid::isSolved() const
{
   return str::all_of(FieldAll, [this](Field f) { return m_candidates[f].isSolved(); });
}

std::ostream &operator<<(std::ostream &os, const Sudoku::CandidatesGrid &cg)
{
   os << ToStrings(cg, &Candidates::ToStrings);
   return os;
}

std::vector<std::string> CandidatesGrid::DifferenceToStrings(const CandidatesGrid &prvGrid,
                                                             const CandidatesGrid &nxtGrid)
{
   CandidatesGrid result;
   Field numChanges = 0;
   for (Field field : FieldAll)
   {
      const Candidates &prv = prvGrid[field];
      const Candidates &nxt = nxtGrid[field];
      if (prv != nxt)
      {
         for (Value value : ValueAll)
         {
            const bool prvContains = prv.contains(value);
            const bool nxtContains = nxt.contains(value);
            MyAssert(!(!prvContains && nxtContains));
            if (prvContains == nxtContains)
            {
               result.removeCandidate(field, value);
            }
         }
         numChanges += result[field].size();
      }
   }

   auto retval = ToStrings(result, &Candidates::ToStringsDifferences);
   retval.push_back("Number of changes = " + std::to_string(numChanges));
   return retval;
}

std::vector<CandidatesGrid> CandidatesGrid::getSymmetricEquivalents(const CandidatesGrid &grid)
{
   // 4 rotations
   std::vector<CandidatesGrid> result{grid};
   result.push_back(Transform(result.back(), &Cell::Rotate));
   result.push_back(Transform(result.back(), &Cell::Rotate));
   result.push_back(Transform(result.back(), &Cell::Rotate));

   // 4 reflections
   result.push_back(Transform(grid, &Cell::ReflectX));
   result.push_back(Transform(grid, &Cell::ReflectY));
   result.push_back(Transform(grid, &Cell::ReflectXY));
   result.push_back(Transform(grid, &Cell::ReflectYX));

   return result;
}

ValueBits CandidatesGrid::getValuesInFields(const CandidatesGrid &grid, std::span<const Field> fields)
{
   ValueBits result;
   for (Field field : fields)
   {
      result |= grid[field].getValues();
   }
   return result;
}
