#include "SubSetPotentialsSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"
#include "Potential.h"
#include "Defines.h"
#include "Combinations.h"

#include<boost/container/static_vector.hpp>
#include <vector>
#include <utility>
#include <iterator>

using namespace Sudoku;

namespace
{
   using ActivePotentials = boost::container::static_vector<Potential*, NumRowColBoxPositions>;

   ActivePotentials getActiveFields(int clusterSize, SubSetPotentials& potentials)
   {
      ActivePotentials active;
      for (auto* ptr : potentials)
      {
         const auto count = ptr->count();
         if (count > 1 && count <= clusterSize) active.push_back(ptr);
      }
      if (active.size() < clusterSize) active.clear();
      return active;
   }

   std::vector<ActivePotentials> getAllPermutations(int clusterSize, SubSetPotentials& potentials)
   {
      std::vector<ActivePotentials> result;
      auto active = getActiveFields(clusterSize, potentials);
      if (active.empty()) return result;

      for (const auto& cmb : Combinations::Get(static_cast<int>(active.size()), clusterSize))
      {
         ActivePotentials perm;
         for (size_t n = 0; n < active.size(); ++n)
         {
            if (cmb.at(n)) perm.push_back(active.at(n));
         }
         result.push_back(perm);
      }
      return result;
   }

   bool FindCluster(int clusterSize, SubSetPotentials& potentials)
   {
      bool changed = false;
      auto trials = getAllPermutations(clusterSize, potentials);
      if (trials.empty()) return changed;

      for (const auto& trial : trials)
      {
         PotentialValues allValues;
         for (const auto& pot : trial)
         {
            const auto potValues = pot->getPotentialValues();
            PotentialValues unionValues;
            std::set_union(allValues.begin(), allValues.end(), potValues.begin(), potValues.end(), std::back_inserter(unionValues));
            allValues = unionValues;
            if (allValues.size() > clusterSize) break;
         }
         if (allValues.size() < clusterSize) throw MyException("FindCluster should not happen");
         if (allValues.size() == clusterSize)
         {
            for (auto* p : potentials)
            {
               if (str::find(trial, p) == trial.end())
               {
                  for (auto val : allValues)
                  {
                     if (p->unset(val)) changed = true;
                  }
               }
            }
            if (changed) return changed;
         }
      }
      return changed;
   }

   bool HandleClusterSizeN(SubSetPotentials& potentials, int clusterSize)
   {
      bool anyChange = false;
      bool changeInIteration = false;
      do
      {
         changeInIteration = FindCluster(clusterSize, potentials);
         if (changeInIteration)
         {
            anyChange = true;
         }
      } while (changeInIteration);
      return anyChange;
   }
} // namespace {}


bool SubSetPotentialsSweepSingles::operator()(SubSetPotentials& potentials)
{
   bool anyChange = false;

   boost::container::static_vector<Value, NumRowColBoxPositions> unsetMe;
   boost::container::static_vector<Potential*, NumRowColBoxPositions> active;

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

bool SubSetPotentialsSweepClusters::operator()(SubSetPotentials& potentials)
{
   for (int clusterSize = 0; clusterSize < 9; ++clusterSize)
   {
      if (HandleClusterSizeN(potentials, clusterSize))
      {
         return true;
      }
   }

   return false;
}