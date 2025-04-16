#include "ProjectToVtk.h"


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

void ProjectToVtk::addCells(const TetrahedronsNodes& tnodes, const IPointCollection3& points, const std::string& name)
{
   std::unique_ptr< Vtk::VtkData> vtkData = std::make_unique< Vtk::VtkData>(GeomDim3, 0, Vtk::Name{ m_projectName, name });
   for (const auto cellId : tnodes.getAllTetrahedrons())
   {
      const auto& cellNodes = tnodes.getTetrahedronNodes(cellId);
      vtkData->addCell(Vtk::CellType::VTK_TETRA, cellNodes, points, {});
   }
   m_vtk.emplace_back(std::move(vtkData));
}

void ProjectToVtk::addEdges(const std::vector<Topology::EdgeNodesSorted>& edges, const IPointCollection3& points, const std::string& name)
{
   std::unique_ptr< Vtk::VtkData> vtkData = std::make_unique< Vtk::VtkData>(GeomDim3, 0, Vtk::Name{ m_projectName, name });
   for (const auto& edge : edges)
   {
      std::array<PointIndex, NumNodesOnEdge> edgeNodes{ edge[0], edge[1] };
      vtkData->addCell(Vtk::CellType::VTK_LINE, edgeNodes, points, {});
   }
   m_vtk.emplace_back(std::move(vtkData));
}