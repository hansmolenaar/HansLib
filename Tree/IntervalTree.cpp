#include "IntervalTree.h"
#include "MyException.h"
#include "VtkData.h"

#include <sstream>
#include <cmath>

using namespace IntervalTree;

namespace
{
   template<int N>
   std::unordered_set<typename Index<N>::Key> RefineToBalance(const IndexTree<N>& tree)
   {
      std::unordered_set<typename Index<N>::Key> result;
      auto action = [&result](const Index<N>& indx) {};
      tree.foreachLeaf(action);
      return result;
   }
}

std::string Statistics::toString() const
{
   std::ostringstream result;
   result << Size;
   result << ", {";
   bool first = true;
   for (auto n : NumberOfLeavesPerLevel)
   {
      if (!first)
      {
         result << ", ";
      }
      first = false;
      result << n;
   }
   result << "}";
   return result.str();
}


std::unique_ptr< Vtk::VtkData> IndexTree<1>::getVtkData() const
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(1, 0);
   std::map<std::array<Rational, 1>, Vtk::NodeIndex> toNodeIndex;

   for (const auto& itr : m_leaves)
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

std::unique_ptr< Vtk::VtkData> IndexTree<2>::getVtkData() const
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(2, 0);
   std::map<std::array<Rational, 2>, Vtk::NodeIndex> toNodeIndex;

   for (const auto& itr : m_leaves)
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


std::unique_ptr< Vtk::VtkData> IndexTree<3>::getVtkData() const
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(3, 0);
   std::map<std::array<Rational, 3>, Vtk::NodeIndex> toNodeIndex;

   for (const auto& itr : m_leaves)
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

template<>
void IndexTree<1>::balance()
{
   auto toRefine = RefineToBalance<1>(*this);
   while (!toRefine.empty())
   {
      auto predicate = [&toRefine](const Index<1>& indx) {return toRefine.contains(indx.getKey()); };
      refineLeaves(predicate);
      toRefine = RefineToBalance<1>(*this);
   }
}