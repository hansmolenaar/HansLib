#pragma once

#include "IntervalTreeVtk.h"
#include "IntervalTreeAction.h"

using namespace IntervalTree;

template<>
std::unique_ptr< Vtk::VtkData> IntervalTree::GetVtkData(const IndexTree<1>& tree)
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(1, 0);
   std::map<std::array<Rational, 1>, Vtk::NodeIndex> toNodeIndex;

   ActionCollectLeaves<1> leaves;
   tree.foreachLeaf(leaves);
   for (const auto* itr : leaves.Leaves)
   {
      std::array<Vtk::NodeIndex, 2> cellNodes{ -1, -1 };
      size_t vertex = 0;
      for (const auto& v : itr->getVerticesInVtkOrder())
      {
         if (!toNodeIndex.contains(v))
         {
            toNodeIndex[v] = static_cast<Vtk::NodeIndex>(toNodeIndex.size());

            std::array<float, 1> coordinates;
            coordinates.at(0) = static_cast<float>(v[0].numerator()) / v[0].denominator();
            result->addNode(coordinates);
         }

         cellNodes.at(vertex) = toNodeIndex.at(v);
         ++vertex;
      }
      result->addCell(Vtk::CellType::VTK_LINE, cellNodes, {});
   }

   return result;
}

template<>
std::unique_ptr< Vtk::VtkData> IntervalTree::GetVtkData(const IndexTree<2>& tree)
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(2, 0);
   std::map<std::array<Rational, 2>, Vtk::NodeIndex> toNodeIndex;

   ActionCollectLeaves<2> leaves;
   tree.foreachLeaf(leaves);
   for (const auto* itr : leaves.Leaves)
   {
      std::array<Vtk::NodeIndex, 4> cellNodes{ -1, -1, -1, -1 };
      size_t vertex = 0;
      for (const auto& v : itr->getVerticesInVtkOrder())
      {
         if (!toNodeIndex.contains(v))
         {
            toNodeIndex[v] = static_cast<Vtk::NodeIndex>(toNodeIndex.size());

            std::array<float, 2> coordinates;
            for (size_t n = 0; n < 2; ++n)
            {
               coordinates.at(n) = static_cast<float>(v[n].numerator()) / v[n].denominator();
            }
            result->addNode(coordinates);
         }

         cellNodes.at(vertex) = toNodeIndex.at(v);
         ++vertex;
      }
      result->addCell(Vtk::CellType::VTK_QUAD, cellNodes, {});
   }

   return result;
}

template<>
std::unique_ptr< Vtk::VtkData> IntervalTree::GetVtkData(const IndexTree<3>& tree)
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(3, 0);
   std::map<std::array<Rational, 3>, Vtk::NodeIndex> toNodeIndex;

   ActionCollectLeaves<3> leaves;
   tree.foreachLeaf(leaves);
   for (const auto* itr : leaves.Leaves)
   {
      std::array<Vtk::NodeIndex, 8> cellNodes{ -1,-1,-1,-1,-1,-1,-1,-1 };
      size_t vertex = 0;
      for (const auto& v : itr->getVerticesInVtkOrder())
      {
         if (!toNodeIndex.contains(v))
         {
            toNodeIndex[v] = static_cast<Vtk::NodeIndex>(toNodeIndex.size());

            std::array<float, 3> coordinates;
            for (size_t n = 0; n < 3; ++n)
            {
               coordinates.at(n) = static_cast<float>(v[n].numerator()) / v[n].denominator();
            }
            result->addNode(coordinates);
         }

         cellNodes.at(vertex) = toNodeIndex.at(v);
         ++vertex;
      }
      result->addCell(Vtk::CellType::VTK_HEXAHEDRON, cellNodes, {});
   }

   return result;
}