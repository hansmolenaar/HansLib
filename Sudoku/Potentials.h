#pragma once
#include "SudokuDefines.h"
#include "Potential.h"
#include <array>
#include <span>

namespace Sudoku
{
   class Potentials
   {
   public:
      Potentials();
      Value getSingleOrUndefined(FieldIndex field) const;
      void setSingle(FieldIndex field, Value value);
      bool isSingle(FieldIndex field) const;
      // Returns: potential changed
      bool unset(FieldIndex field, Value value);
     
   private:
      std::array<Potential, NumFields> m_potentials;
   };

}