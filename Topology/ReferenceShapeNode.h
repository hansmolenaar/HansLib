#pragma once

#include "IReferenceShape.h"

class ReferenceShapeNode : public IReferenceShape
{
public:
    const ITopologicalAdjacencies& getAdjacencies() const override;
};
