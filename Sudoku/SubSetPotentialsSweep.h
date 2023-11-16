#pragma once
#include "SudokuDefines.h"
#include "Potentials.h"

namespace Sudoku
{
   class ISubSetPotentialsSweep
   {
   public:
      virtual ~ISubSetPotentialsSweep() noexcept = default;
      virtual bool operator()(SubSetPotentials& potentials) = 0;
   };
   
   class SubSetPotentialsSweepSingles : public ISubSetPotentialsSweep
   {
   public:
      virtual bool operator()(SubSetPotentials& potentials) override;
   };

   class SubSetPotentialsSweepClusters : public ISubSetPotentialsSweep
   {
   public:
      virtual bool operator()(SubSetPotentials& potentials) override;
   };
}