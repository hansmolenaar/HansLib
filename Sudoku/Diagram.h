#pragma once

#include "Potential.h"
#include "Potentials.h"
#include <ostream>
#include <unordered_map>

namespace Sudoku
{
class Diagram
{
  public:
    static Diagram Create(const std::array<Value, NumFields> &values);
    static Diagram Create(const std::unordered_map<FieldIndex, Value> &input);
    static Diagram Create(const Potentials &values);
    Value operator()(FieldIndex field) const;
    ValueSet getValues(RowColBoxType type, RowColBoxIndex subSetIndex) const;
    Potentials getPotentials() const;
    std::string toString() const;
    bool isSolved() const;

  private:
    Diagram(std::array<Value, NumFields> values);
    std::array<Value, NumFields> m_state;
};

}; // namespace Sudoku

std::ostream &operator<<(std::ostream &os, const Sudoku::Diagram &diagram);