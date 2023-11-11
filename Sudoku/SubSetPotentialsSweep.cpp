#include "SubSetPotentialsSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"
#include "Potential.h"
#include "Defines.h"

#include<boost/container/static_vector.hpp>
#include <vector>
#include <utility>

using namespace Sudoku;

bool SubSetPotentialsSweepSingles::operator()(SubSetPotentials& potentials)
{
   bool anyChange = false;

   boost::container::static_vector<Value, SubSetSize> unsetMe;
   boost::container::static_vector<Potential*, SubSetSize> active;

   // Collect values that are unique
   for (auto pot : potentials)
   {
      if (pot->isSingle())
      {
         unsetMe.push_back(pot->getSingleValue());
      }
      else
      {
         active.push_back(pot);
      }
   }

   if (unsetMe.empty()) anyChange;

   for (auto pot : active)
   {
      for (auto value : unsetMe)
      {
         if (pot->unset(value))
         {
            anyChange = true;
         }
      }
   }

   return anyChange;
}

bool SubSetPotentialsSweepPairs::operator()(SubSetPotentials& potentials)
{
   boost::container::static_vector<Potential*, SubSetSize> active;
   for (auto pot : potentials)
   {
      if (pot->count() == 2)
      {
         active.push_back(pot);
      }
   }

   if (!active.empty())
   {

      int hello = 1;
      std::vector < std::pair<Value,Value> > values;
      for (auto pot : active)
      {
         auto valuePair = pot->getPotentialValues();
         str::sort(valuePair);
         values.emplace_back(std::make_pair(valuePair.at(0), valuePair.at(1)));
      }
      if (values.size() > 1)
      {
         hello += 1;
      }

   }

   return false;
}