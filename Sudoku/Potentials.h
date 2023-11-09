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
      Value GetSingleOrUndefined(FieldIndex field) const;
      void SetSingle(FieldIndex field, Value value);
      void Set(FieldIndex field, Potential potential);
      bool isSingle(FieldIndex field) const;
      // Returns: potential changed
      bool unset(FieldIndex field, Value value);
     
   private:
      std::array<Potential, NumFields> m_potentials;
   };

}