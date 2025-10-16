#include "IntervalTreeAction.h"
#include "IntervalTreeVtk.h"
#include "UniqueHashedPointCollection.h"

using namespace IntervalTree;

namespace
{
   template<size_t N>
   std::unique_ptr<Vtk::VtkData> getVtkData(const IndexTree<N>& tree, Vtk::CellType cellType, const Vtk::Name& name)
   {
      std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(N, 0, name);
      UniqueHashedPointCollection<Rational, N> pointCollection;

      ActionCollectLeaves<N> leaves;
      tree.foreachLeaf(leaves);
      for (const auto* itr : leaves.Leaves)
      {
         std::array<PointIndex, 1 << N> cellNodes;
         for (size_t vertex = 0; const auto & v : itr->getVerticesInVtkOrder())
         {
            cellNodes.at(vertex) = pointCollection.addIfNew(v);
            ++vertex;
         }
         result->addCell(cellType, cellNodes, pointCollection, {});
      }

      return result;
   }
}

template<>
std::unique_ptr< Vtk::VtkData> IntervalTree::GetVtkData(const IndexTree<1>& tree, const Vtk::Name& name)
{
   return getVtkData(tree, Vtk::CellType::VTK_LINE, name);
}

template<>
std::unique_ptr< Vtk::VtkData> IntervalTree::GetVtkData(const IndexTree<2>& tree, const Vtk::Name& name)
{
   return getVtkData(tree, Vtk::CellType::VTK_QUAD, name);
}

template<>
std::unique_ptr< Vtk::VtkData> IntervalTree::GetVtkData(const IndexTree<3>& tree, const Vtk::Name& name)
{
   return getVtkData(tree, Vtk::CellType::VTK_HEXAHEDRON, name);
}
