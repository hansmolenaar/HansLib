#include "Manifold0Reconstruction.h"
#include "MyException.h"

void Manifold0Reconstruction::Check(MeshGeneration::NodeIndex manifoldPoint, const MeshGeneration::TrianglesNodes& trianglesNodes)
{
   if (!trianglesNodes.isKnownNodeId(manifoldPoint))
   {
      throw MyException("Manifold0Reconstruction::Check unknown node " + std::to_string(manifoldPoint));
   }
}