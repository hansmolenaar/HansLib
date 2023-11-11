#include "SubSetPotentialsSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"

#include<boost/container/static_vector.hpp>

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
   return false;
}