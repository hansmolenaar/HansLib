#pragma once

#include "IReferenceShape.h"
#include <memory>

class ReferenceShape : public IReferenceShape
{
public:
   ReferenceShape(std::unique_ptr<ITopologicalAdjacencies>&&);
   const ITopologicalAdjacencies& getAdjacencies() const override;
private:
   std::unique_ptr<ITopologicalAdjacencies> m_adjacencies;
};
