#include "TestModels.h"

using namespace Sudoku;

#if false
const std::array<Value, NumFields> diagramValues =
{
   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,

   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,

   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,
};
#endif

Diagram TestModels::getEmpty()
{
    return Diagram::Create(std::unordered_map<FieldIndex, Value>{});
}

Diagram TestModels::getSolved()
{
    const std::array<Value, NumFields> diagramValues = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 4, 5, 6, 7, 8, 9, 1, 2, 3, 7, 8, 9, 1, 2, 3, 4, 5, 6,

        2, 3, 4, 5, 6, 7, 8, 9, 1, 5, 6, 7, 8, 9, 1, 2, 3, 4, 8, 9, 1, 2, 3, 4, 5, 6, 7,

        3, 4, 5, 6, 7, 8, 9, 1, 2, 6, 7, 8, 9, 1, 2, 3, 4, 5, 9, 1, 2, 3, 4, 5, 6, 7, 8,
    };
    return Diagram::Create(diagramValues);
}

Diagram TestModels::getOneStar1()
{
    const std::array<Value, NumFields> diagramValues = {
        4, 9, 6, 0, 0, 2, 1, 8, 5, 0, 7, 2, 1, 0, 0, 4, 9, 0, 1, 0, 0, 4, 9, 0, 3, 7, 2,

        9, 4, 0, 6, 2, 7, 8, 5, 1, 6, 2, 7, 0, 5, 1, 9, 4, 0, 0, 5, 1, 0, 0, 3, 0, 0, 7,

        0, 0, 4, 0, 3, 9, 5, 1, 8, 0, 0, 9, 5, 1, 8, 7, 0, 0, 0, 0, 8, 7, 0, 0, 2, 0, 0,
    };

    return Diagram::Create(diagramValues);
}

Diagram TestModels::getOneStar2()
{
    const std::array<Value, NumFields> diagramValues = {
        2, 0, 8, 7, 0, 0, 0, 4, 3, 0, 1, 5, 9, 4, 3, 2, 0, 8, 9, 0, 0, 2, 0, 0, 7, 1, 5,

        0, 2, 0, 0, 0, 1, 4, 3, 0, 8, 5, 1, 0, 0, 9, 0, 2, 0, 4, 3, 0, 0, 0, 7, 8, 5, 0,

        1, 8, 2, 5, 7, 6, 3, 0, 0, 0, 7, 0, 0, 9, 4, 0, 8, 2, 3, 9, 4, 1, 8, 2, 5, 0, 6,
    };

    return Diagram::Create(diagramValues);
}

Diagram TestModels::getTwoStar1()
{
    const std::array<Value, NumFields> diagramValues = {
        0, 3, 0, 0, 8, 7, 4, 6, 5, 9, 8, 0, 0, 6, 0, 0, 3, 1, 0, 6, 5, 0, 3, 0, 0, 8, 7,

        0, 0, 0, 1, 7, 0, 6, 5, 4, 1, 7, 0, 6, 0, 0, 0, 0, 9, 6, 0, 0, 0, 0, 9, 1, 7, 8,

        8, 1, 2, 7, 0, 0, 5, 0, 0, 0, 9, 3, 5, 4, 6, 0, 0, 2, 0, 4, 0, 0, 0, 0, 0, 0, 3,
    };

    return Diagram::Create(diagramValues);
}

Diagram TestModels::getThreeStar1()
{
    const std::array<Value, NumFields> diagramValues = {
        9, 0, 0, 0, 3, 8, 7, 0, 5, 6, 3, 0, 0, 1, 5, 0, 0, 0, 7, 1, 0, 9, 4, 0, 0, 3, 0,

        0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 8, 0, 1, 5, 7, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,

        3, 2, 0, 0, 0, 4, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 9, 0, 6, 4,
    };
    return Diagram::Create(diagramValues);
}

Diagram TestModels::getThreeStar2()
{
    const std::array<Value, NumFields> diagramValues = {
        7, 0, 5, 0, 0, 0, 6, 0, 0, 0, 9, 0, 6, 3, 0, 0, 0, 5, 6, 0, 2, 7, 0, 5, 0, 9, 0,

        0, 0, 0, 0, 1, 9, 0, 0, 0, 0, 0, 0, 3, 2, 0, 0, 7, 0, 0, 0, 6, 0, 0, 0, 5, 1, 0,

        9, 0, 0, 1, 0, 4, 0, 0, 3, 0, 0, 4, 2, 6, 0, 9, 5, 7, 0, 6, 0, 0, 0, 0, 1, 0, 0,
    };
    return Diagram::Create(diagramValues);
}

Diagram TestModels::getFourStar1()
{
    const std::array<Value, NumFields> diagramValues = {
        0, 5, 0, 6, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 5, 0, 6, 7, 3,

        0, 0, 0, 2, 3, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 9, 6, 4, 0, 1, 0, 0, 0, 0, 0, 0,

        7, 0, 9, 0, 0, 0, 8, 1, 0, 0, 0, 0, 8, 0, 4, 0, 2, 0, 0, 1, 0, 0, 0, 9, 0, 0, 0,
    };
    return Diagram::Create(diagramValues);
}

Diagram TestModels::getFourStar2()
{
    const std::array<Value, NumFields> diagramValues = {
        2, 0, 4, 0, 8, 0, 1, 0, 0, 0, 8, 0, 0, 7, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,

        0, 0, 0, 0, 0, 8, 0, 0, 0, 4, 9, 0, 0, 0, 0, 0, 2, 0, 7, 0, 1, 3, 0, 5, 0, 9, 0,

        0, 0, 0, 9, 0, 0, 0, 0, 7, 0, 5, 3, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 0,
    };
    return Diagram::Create(diagramValues);
}

Diagram TestModels::getFourStar3()
{
    const std::array<Value, NumFields> diagramValues = {
        0, 0, 0, 0, 0, 0, 9, 0, 0, 2, 0, 3, 0, 0, 4, 0, 1, 8, 0, 7, 4, 0, 0, 0, 0, 0, 0,

        1, 0, 2, 8, 0, 5, 0, 4, 0, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 3, 0,

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 9, 7, 0, 0, 0, 4, 0, 0, 5, 0, 0, 0, 2, 1,
    };
    return Diagram::Create(diagramValues);
}

Diagram TestModels::getBiValue1()
{
    const std::array<Value, NumFields> diagramValues = {
        0, 9, 3, 8, 2, 4, 5, 6, 0, 0, 8, 5, 6, 0, 0, 0, 0, 2, 2, 0, 6, 0, 7, 5, 0, 0, 8,

        3, 2, 1, 7, 6, 9, 8, 4, 5, 0, 0, 0, 2, 5, 8, 3, 0, 0, 5, 7, 8, 0, 4, 0, 2, 9, 6,

        8, 5, 0, 0, 1, 6, 7, 2, 3, 0, 0, 7, 0, 8, 2, 6, 5, 0, 0, 0, 2, 5, 0, 7, 1, 8, 0,
    };
    return Diagram::Create(diagramValues);
}