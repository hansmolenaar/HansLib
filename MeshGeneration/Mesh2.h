#pragma once

#include "IndexTreeToSimplices2.h"
#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"

namespace MeshGeneration2
{
   class Mesh2
   {
   public:

      void setBaseTriangles(const IndexTreeToSimplices2::Triangles& triangles);
      const IndexTreeToSimplices2::Triangles& getBaseTriangles() const;

      MeshGeneration::TrianglesNodes& getTriangles();
      const MeshGeneration::TrianglesNodes& getTriangles() const;

      std::unique_ptr<IDynamicUniquePointCollection2>& createPoints();
      const IUniquePointCollection2& getPoints() const;
      IDynamicUniquePointCollection2& getSetPoints();

   private:
      IndexTreeToSimplices2::Triangles m_baseTriangles;
      MeshGeneration::TrianglesNodes m_trianglesNodes;
      std::unique_ptr<IDynamicUniquePointCollection2> m_points;
   };

}