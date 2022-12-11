#pragma once

#include "ITopologicalAdjacencies.h"
#include "IReferenceShapeHyperCube.h"

#include <vector>
#include <memory>

class GridTopology
{
public:
   explicit GridTopology(const std::vector<int>& cellDimensions);
   const ITopologicalAdjacencies& getAdjacencies() const;
   const IReferenceShapeHyperCube& getShape() const;

private:
   std::unique_ptr<ITopologicalAdjacencies> m_adjacencies;
   const IReferenceShapeHyperCube& m_shape;
};
