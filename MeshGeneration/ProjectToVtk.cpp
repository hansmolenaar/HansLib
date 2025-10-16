#include "Boundary1.h"
#include "CycleNodes.h"
#include "CycleNodes.h"
#include "EdgeNodesSorted.h"
#include "Functors.h"
#include "Manifold1Reconstruction.h"
#include "PathNodes.h"
#include "PathNodes.h"
#include "ProjectToVtk.h"
#include "TetrahedronsNodes.h"
#include "TrianglesNodes.h"

using namespace MeshGeneration;
using namespace Topology;

ProjectToVtk::ProjectToVtk(const std::string& projectName) : m_projectName(projectName)
{
}

std::vector<const Vtk::VtkData*> ProjectToVtk::get() const
{
   std::vector<const Vtk::VtkData*> result(m_vtk.size());
   str::transform(m_vtk, result.begin(), [](const auto& up) {return up.get(); });
   return result;
}

void ProjectToVtk::addTetrahedrons(const TetrahedronsNodes& tnodes, const IPointCollection3& points, const std::string& name)
{
   std::unique_ptr< Vtk::VtkData> vtkData = std::make_unique< Vtk::VtkData>(GeomDim3, 0, Vtk::Name{ m_projectName, name });
   for (const auto cellId : tnodes.getAllTetrahedrons())
   {
      const auto& cellNodes = tnodes.getTetrahedronNodes(cellId);
      vtkData->addCell(Vtk::CellType::VTK_TETRA, cellNodes, points, {});
   }
   m_vtk.emplace_back(std::move(vtkData));
}

void ProjectToVtk::addTetrahedronsAndBoundaries(const TetrahedronsNodes& tnodes, const IPointCollection3& points, const std::string& name)
{
   addTetrahedrons(tnodes, points, name);
   addTrianglesAndBoundaries(tnodes.getBoundaryFaces(), points, name + "_BDY");
}

template void ProjectToVtk::addEdges<Topology::EdgeNodesDirected, 2>(const std::vector<EdgeNodesDirected>& edges, const IPointCollection<double, 2>& points, const std::string& name);
template void ProjectToVtk::addEdges<Topology::EdgeNodesSorted, 2>(const std::vector<EdgeNodesSorted>& edges, const IPointCollection<double, 2>& points, const std::string& name);
template void ProjectToVtk::addEdges<Topology::EdgeNodesDirected, 3>(const std::vector<EdgeNodesDirected>& edges, const IPointCollection<double, 3>& points, const std::string& name);
template void ProjectToVtk::addEdges<Topology::EdgeNodesSorted, 3>(const std::vector<EdgeNodesSorted>& edges, const IPointCollection<double, 3>& points, const std::string& name);

template<typename T, size_t N>
void ProjectToVtk::addEdges(const std::vector<T>& edges, const IPointCollection<double, N>& points, const std::string& name)
{
   std::unique_ptr< Vtk::VtkData> vtkData = std::make_unique< Vtk::VtkData>(N, 0, Vtk::Name{ m_projectName, name });
   for (const auto& edge : edges)
   {
      std::array<PointIndex, NumCornersOnEdge> edgeNodes{ edge[0], edge[1] };
      vtkData->addCell(Vtk::CellType::VTK_LINE, edgeNodes, points, {});
   }
   m_vtk.emplace_back(std::move(vtkData));
}

template void ProjectToVtk::addReconstructions<2>(const std::vector<const IManifoldReconstruction*>& reconstructions, const IPointCollection<double, 2>& points);
template void ProjectToVtk::addReconstructions<3>(const std::vector<const IManifoldReconstruction*>& reconstructions, const IPointCollection<double, 3>& points);

template<size_t N>
void ProjectToVtk::addReconstructions(const std::vector<const IManifoldReconstruction*>& reconstructions, const IPointCollection<double, N>& points)
{
   auto cast = [](const IManifoldReconstruction* reconstruction) { return dynamic_cast<const Manifold1Reconstruction*>(reconstruction); };
   for (const auto* manifold1 : reconstructions | stv::transform(cast) | stv::filter(Functors::PointerIsNotNull()))
   {
      addBoundary1(manifold1->getReconstruction(), points, manifold1->getManifoldId().getName());
   }

}

template void ProjectToVtk::addBoundary1<2>(const Boundary1& reconstruction, const IPointCollection<double, 2>& points, const std::string& manifoldName);
template void ProjectToVtk::addBoundary1<3>(const Boundary1& reconstruction, const IPointCollection<double, 3>& points, const std::string& manifoldName);

template<size_t N>
void ProjectToVtk::addBoundary1(const Boundary1& reconstruction, const IPointCollection<double, N>& points, const std::string& manifoldName)
{
   std::vector<std::unique_ptr<Vtk::VtkData>> result;
   if (!reconstruction.getSingletons().empty()) throw MyException("ProjectToVtk::addBoundary1 unexpected singletons");

   for (int count = 0; const auto & path : reconstruction.getPaths())
   {
      ++count;
      addPath(path, points, manifoldName + "_path_" + std::to_string(count));
   }

   for (int count = 0; const auto & cycle : reconstruction.getCycles())
   {
      ++count;
      addCycle(cycle, points, manifoldName + "_cycle_" + std::to_string(count));
   }

}

template void ProjectToVtk::addPath<2>(const PathNodes& path, const IPointCollection<double, 2>& points, const std::string& name);
template void ProjectToVtk::addPath<3>(const PathNodes& path, const IPointCollection<double, 3>& points, const std::string& name);

template<size_t N>
void ProjectToVtk::addPath(const PathNodes& path, const IPointCollection<double, N>& points, const std::string& name)
{
   addEdges(path.getEdges(), points, name);
}

template void ProjectToVtk::addCycle<2>(const CycleNodes& cycle, const IPointCollection<double, 2>& points, const std::string& name);
template void ProjectToVtk::addCycle<3>(const CycleNodes& cycle, const IPointCollection<double, 3>& points, const std::string& name);

template<size_t N>
void ProjectToVtk::addCycle(const CycleNodes& cycle, const IPointCollection<double, N>& points, const std::string& name)
{
   addEdges(cycle.getEdges(), points, name);
}

template void ProjectToVtk::addTriangles<2>(const TrianglesNodes& tnodes, const IPointCollection<double, 2>& points, const std::string& name);
template void ProjectToVtk::addTriangles<3>(const TrianglesNodes& tnodes, const IPointCollection<double, 3>& points, const std::string& name);

template<size_t N>
void ProjectToVtk::addTriangles(const TrianglesNodes& tnodes, const IPointCollection<double, N>& points, const std::string& name)
{
   std::unique_ptr< Vtk::VtkData> vtkData = std::make_unique< Vtk::VtkData>(N, 0, Vtk::Name{ m_projectName, name });
   for (auto cellId : tnodes.getAllTriangles())
   {
      const auto trg = tnodes.getTriangleNodes(cellId);
      vtkData->addCell(Vtk::CellType::VTK_TRIANGLE, std::array<NodeIndex, NumCornersOnTriangle>{trg[0], trg[1], trg[2]}, points, {});
   }
   m_vtk.emplace_back(std::move(vtkData));
}

void ProjectToVtk::addTrianglesAndReconstructions(const TrianglesNodes& tnodes, const std::vector<const IManifoldReconstruction*>& reconstructions, const IPointCollection2& points)
{
   addTriangles(tnodes, points, "mesh");
   addReconstructions(reconstructions, points);
}

template void ProjectToVtk::addTrianglesAndBoundaries<2>(const TrianglesNodes& tnodes, const IPointCollection<double, 2>& points, const std::string& name);
template void ProjectToVtk::addTrianglesAndBoundaries<3>(const TrianglesNodes& tnodes, const IPointCollection<double, 3>& points, const std::string& name);

template<size_t N>
void ProjectToVtk::addTrianglesAndBoundaries(const TrianglesNodes& tnodesAll, const IPointCollection<double, N>& points, const std::string& name)
{
   addTriangles(tnodesAll, points, name);

   const auto connectedTriangleIds = tnodesAll.splitInEdgeConnectedComponents();
   for (int subSetId = 0; const auto & tnodesIds : connectedTriangleIds)
   {
      // get edge connected subset
      const TrianglesNodes tnodes = tnodesAll.createSubSet(tnodesIds);
      const std::string nameTriangles = name + "_PART_" + std::to_string(subSetId);
      addTriangles(tnodes, points, nameTriangles);
      addBoundariesOfConnectedTriangles(tnodes, points, nameTriangles);
      ++subSetId;
   }
}

template void ProjectToVtk::addBoundariesOfConnectedTriangles<2>(const TrianglesNodes& tnodes, const IPointCollection<double, 2>& points, const std::string& name);
template void ProjectToVtk::addBoundariesOfConnectedTriangles<3>(const TrianglesNodes& tnodes, const IPointCollection<double, 3>& points, const std::string& name);

template<size_t N>
void ProjectToVtk::addBoundariesOfConnectedTriangles(const TrianglesNodes& tnodes, const IPointCollection<double, N>& points, const std::string& name)
{
   const Boundary1 bdy1 = Boundary1::createFromBoundaryEdges(tnodes);
   addBoundary1(bdy1, points, name + "_BDY");
}
