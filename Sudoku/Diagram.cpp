#include "Diagram.h"
#include "Defines.h"
#include "FieldInfoStatic.h"
#include "Functors.h"
#include "MyException.h"
#include "Potential.h"
#include "ValueSetUtils.h"
#include <algorithm>
#include <sstream>
#include <unordered_set>

using namespace Sudoku;

Diagram::Diagram(std::array<Value, NumFields> values) : m_state(std::move(values))
{
}

Diagram Diagram::Create(const std::unordered_map<FieldIndex, Value> &input)
{
    std::array<Value, NumFields> values;
    str::fill(values, ValueUndefined);
    for (auto iv : input)
    {
        FieldInfoStatic::CheckValue(iv.second);
        values.at(iv.first) = iv.second;
    }

    Diagram result(values);
    // Check correctness
    result.isSolved();
    return result;
}

Diagram Diagram::Create(const Potentials &values)
{
    std::unordered_map<FieldIndex, Value> map;
    for (auto field : FieldInfoStatic::getAllFields())
    {
        const auto value = values.getSingleOrUndefined(field);
        if (value != ValueUndefined)
        {
            map[field] = value;
        }
    }
    return Create(map);
}

Diagram Diagram::Create(const std::array<Value, NumFields> &values)
{
    std::unordered_map<FieldIndex, Value> map;
    FieldIndex field = 0;
    for (Value value : values)
    {
        if (value != ValueUndefined)
        {
            map[field] = value;
        }
        ++field;
    }
    return Create(map);
}

Value Diagram::operator()(FieldIndex field) const
{
    return m_state.at(field);
}

std::ostream &operator<<(std::ostream &os, const Sudoku::Diagram &diagram)
{
    for (RowColBoxIndex row = 0; row < NumRowColBox; ++row)
    {
        os << "|";
        for (RowColBoxIndex col = 0; col < NumRowColBox; ++col)
        {
            const FieldIndex field = FieldInfoStatic::RowColToField(row, col);
            const Value value = diagram(field);
            os << " " << (value == ValueUndefined ? "-" : std::to_string(value)) + " ";
        }
        os << "|\n";
    }
    return os;
}

Sudoku::Potentials Diagram::getPotentials() const
{
    Potentials result;
    for (auto field : FieldInfoStatic::getAllFields())
    {
        if (m_state.at(field) != ValueUndefined)
        {
            result.setSingle(field, m_state.at(field));
        }
    }
    return result;
}

std::string Diagram::toString() const
{
    std::ostringstream stream;
    stream << *this;
    return stream.str();
}

bool Diagram::isSolved() const
{
    bool succes = true;
    for (auto type : RowColBoxTypeAll)
    {
        for (auto rcbIndex : RowColBoxAll)
        {
            const ValueSet values = getValues(type, rcbIndex);
            if (!ValueSetUtils::IsSolved(values))
            {
                // Do not break loop, also test for correctness
                succes = false;
            }
        }
    }

    return succes;
}

ValueSet Diagram::getValues(RowColBoxType type, RowColBoxIndex subSetIndex) const
{
    const auto &fields = FieldInfoStatic::GetFieldSet(type, subSetIndex);
    ValueSet values;
    str::transform(fields, values.begin(), [this](FieldIndex f) { return (*this)(f); });
    return values;
}
