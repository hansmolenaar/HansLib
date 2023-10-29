#pragma once

#include "Potential.h"
#include <unordered_map>
#include <ostream>

namespace Sudoku
{
   class Diagram
   {
   public:
      static Diagram Create(const std::unordered_map<FieldIndex, Value>& input);
      static Diagram Create(const  std::array<Value, NumFields> values);
      Value operator()(FieldIndex field) const;
      std::array<Potential, NumFields> getPotentials() const;
      std::string toString() const;
      bool isSolved() const;
      const std::array<Value, NumFields>& getState() const;
   private:
      Diagram(std::array<Value, NumFields> values);
      std::array<Value, NumFields> m_state;
   };

};

std::ostream& operator<<(std::ostream& os, const Sudoku::Diagram& diagram);