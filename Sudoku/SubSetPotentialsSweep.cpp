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
   using ActivePotentials = boost::container::static_vector<Potential*, SubSetSize>;

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


   bool Handle22(const std::vector<const Potential*> potentialPairs, SubSetPotentials& potentials)
   {
      bool anyChange = false;
      bool changeInIteration = false;
      do
      {
          changeInIteration = FindCluster(2, potentials);
         if (changeInIteration)
         {
            anyChange = true;
         }
      } while (changeInIteration);
      return anyChange;
   }

   bool Handle223(const std::vector<const Potential*> potentialPairs, const std::vector<const Potential*> potentialTriplets, SubSetPotentials& potentials)
   {
      bool changed = false;
      if (potentialPairs.size() < 2) return changed;
      if (potentialTriplets.empty()) return changed;
      for (size_t n0 = 0; n0 < potentialPairs.size() - 1; ++n0)
      {
         const auto* pot2_0 = potentialPairs.at(n0);
         for (size_t n1 = n0 + 1; n1 < potentialPairs.size(); ++n1)
         {
            const auto* pot2_1 = potentialPairs.at(n1);
            const auto common2 = Potential::getUnion(*pot2_0, *pot2_1);
            if (common2.size() == 3)
            {
               for (const auto* pot3 : potentialTriplets)
               {
                  const auto values3 = pot3->getPotentialValues();
                  if (str::equal(common2, values3))
                  {
                     for (auto* pot : potentials)
                     {
                        if (pot == pot2_0 || pot == pot2_1 || pot == pot3)  continue;
                        for (auto val : values3)
                        {
                           if (pot->unset(val))
                           {
                              changed = true;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
      return changed;
   }


   bool Handle2234(const std::vector<const Potential*> potentialPairs, const std::vector<const Potential*> potentialTriplets, const std::vector<const Potential*> potentialQuartets, SubSetPotentials& potentials)
   {
      bool changed = false;
      if (potentialPairs.size() < 2) return changed;
      if (potentialTriplets.empty()) return changed;
      if (potentialQuartets.empty()) return changed;
      for (size_t n0 = 0; n0 < potentialPairs.size() - 1; ++n0)
      {
         const auto* pot2_0 = potentialPairs.at(n0);
         for (size_t n1 = n0 + 1; n1 < potentialPairs.size(); ++n1)
         {
            const auto* pot2_1 = potentialPairs.at(n1);
            const auto common2 = Potential::getUnion(*pot2_0, *pot2_1);
            if (common2.size() == 4)
            {
               for (const auto* pot4 : potentialQuartets)
               {
                  const auto values4 = pot4->getPotentialValues();
                  if (str::equal(common2, values4))
                  {
                     for (auto* pot3 : potentialTriplets)
                     {
                        const auto values3 = pot3->getPotentialValues();
                        const bool allContained = str::all_of(values3, [&values4](Value value) {return str::find(values4, value) != values4.end();  });
                        if (allContained)
                        {
                           for (auto* pot : potentials)
                           {
                              if (pot == pot2_0 || pot == pot2_1 || pot == pot3 || pot == pot4)  continue;
                              for (auto val : values4)
                              {
                                 if (pot->unset(val))
                                 {
                                    changed = true;
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
      return changed;
   }
}

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
   std::map<int, std::vector<const Potential*>> bySize;
   for (const auto* pot : potentials)
   {
      const auto count = pot->count();
      if (count == 1) continue;
      bySize[count].push_back(pot);
   }

   if (Handle22(bySize[2], potentials))
   {
      return true;
   }

   if (Handle223(bySize[2], bySize[3], potentials))
   {
      return true;
   }

   if (Handle2234(bySize[2], bySize[3], bySize[4], potentials))
   {
      return true;
   }

   return false;
}