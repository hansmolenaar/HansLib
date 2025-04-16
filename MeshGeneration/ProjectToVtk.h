#pragma once

#include "CycleNodes.h"
#include "EdgeNodesSorted.h"
#include "PathNodes.h"
#include "TetrahedronsNodes.h"
#include "TrianglesNodes.h"
#include "VtkData.h"

namespace MeshGeneration
{

   class ProjectToVtk
   {
   public:
      explicit ProjectToVtk(const std::string& projectName);

      std::vector<const Vtk::VtkData*> get() const;

      void addCells(const TetrahedronsNodes& tnodes, const IPointCollection3& points, const std::string& name);
      void addEdges(const std::vector<Topology::EdgeNodesSorted>& edges, const IPointCollection3& points, const std::string& name);

   private:
      std::string m_projectName;
      std::vector<std::unique_ptr<Vtk::VtkData>> m_vtk;
   };

}