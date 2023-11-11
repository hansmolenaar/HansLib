#pragma once
#include "SudokuDefines.h"
#include "Potential.h"
#include <array>

namespace Sudoku
{
   using SubSetPotentials = std::array<Potential*, SubSetSize>;

   class Potentials
   {
   public:
      Potentials();
      Value getSingleOrUndefined(FieldIndex field) const;
      void setSingle(FieldIndex field, Value value);
      bool isSingle(FieldIndex field) const;
      // Returns: potential changed
      bool unset(FieldIndex field, Value value);
      const Potential& get(FieldIndex field) const;

      SubSetPotentials getSubSetPotentials(SubSetType type, SubSetIndex index);
      static std::string toString(const SubSetPotentials potentials);
      std::string toString(); // TODO const correctness
     
   private:
      std::array<Potential, NumFields> m_potentials;
   };

}