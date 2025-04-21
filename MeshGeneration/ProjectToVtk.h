#pragma once

#include "EdgeNodesSorted.h"
#include "IManifoldReconstruction.h"
#include "VtkData.h"

namespace Topology
{
   class PathNodes;
   class CycleNodes;
}

namespace MeshGeneration
{
   class Boundary1;
   class TetrahedronsNodes;
   class TrianglesNodes;

   class ProjectToVtk
   {
   public:
      explicit ProjectToVtk(const std::string& projectName);

      std::vector<const Vtk::VtkData*> get() const;

      void addTetrahedrons(const TetrahedronsNodes& tnodes, const IPointCollection3& points, const std::string& name);

      void addTetrahedronsAndBoundaries(const TetrahedronsNodes& tnodes, const IPointCollection3& points, const std::string& name);

      void addTrianglesAndReconstructions(const TrianglesNodes& tnodes, const std::vector<const IManifoldReconstruction*>& reconstructions, const IPointCollection2& points);

      template<int N>
      void addTriangles(const TrianglesNodes& tnodes, const IPointCollection<double, N>& points, const std::string& name);

      template<int N>
      void addTrianglesAndBoundaries(const TrianglesNodes& tnodes, const IPointCollection<double, N>& points, const std::string& name);

      template<typename T, int N>
      void addEdges(const std::vector<T>& edges, const IPointCollection<double, N>& points, const std::string& name);

      template<int N>
      void addReconstructions(const std::vector<const IManifoldReconstruction*>& reconstructions, const IPointCollection<double, N>& points);

      template<int N>
      void addBoundary1(const Boundary1& reconstruction, const IPointCollection<double, N>& points, const std::string& manifoldName);

      template<int N>
      void addPath(const Topology::PathNodes& path, const IPointCollection<double, N>& points, const std::string& name);

      template<int N>
      void addCycle(const Topology::CycleNodes& cycle, const IPointCollection<double, N >& points, const std::string& name);

   private:
      template<int N>
      void addBoundariesOfConnectedTriangles(const TrianglesNodes& tnodes, const IPointCollection<double, N>& points, const std::string& name);

      std::string m_projectName;
      std::vector<std::unique_ptr<Vtk::VtkData>> m_vtk;
   };

}