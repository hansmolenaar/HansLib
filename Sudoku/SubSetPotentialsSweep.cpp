#include "SubSetPotentialsSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"
#include "Potential.h"
#include "Defines.h"
#include "Combinations.h"

#include<boost/container/static_vector.hpp>
#include <vector>
#include <utility>

using namespace Sudoku;

namespace
{
   bool FindCluster(int clusterSize, SubSetPotentials& potentials)
   {
      bool changed = false;
      boost::container::static_vector<Potential*, SubSetSize> active;
      for (auto* ptr : potentials)
      {
         if (ptr->count() <= clusterSize) active.push_back(ptr);
      }
      if (active.size() < clusterSize) return changed;

      for (const auto& cmb : Combinations::Get(active.size(), clusterSize)
      {

      }
   }


   bool Handle22(const std::vector<const Potential*> potentialPairs, SubSetPotentials& potentials)
   {
      bool changed = false;
      if (potentialPairs.size() <= 1) return changed;
      for (size_t n0 = 0; n0 < potentialPairs.size() - 1; ++n0)
      {
         const auto* pot0 = potentialPairs.at(n0);
         for (size_t n1 = n0 + 1; n1 < potentialPairs.size(); ++n1)
         {
            const auto* pot1 = potentialPairs.at(n1);
            const auto common = Potential::getIntersection(*pot0, *pot1);
            if (common.size() == 2)
            {
               for (auto* pot : potentials)
               {
                  if (pot == pot0 || pot == pot1)  continue;
                  if (pot->unset(common.front()))
                  {
                     changed = true;
                  }
                  if (pot->unset(common.back()))
                  {
                     changed = true;
                  }
               }
            }
         }
      }
      return changed;
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