#pragma once

#include "IntervalTree.h"
#include "VtkData.h"

#include <memory>

namespace IntervalTree
{
   template<int N>
   std::unique_ptr<Vtk::VtkData> GetVtkData(const IndexTree<N>& tree);

   template<>
   std::unique_ptr< Vtk::VtkData> GetVtkData(const IndexTree<1>& tree);

   template<>
   std::unique_ptr< Vtk::VtkData> GetVtkData(const IndexTree<2>& tree);

   template<>
   std::unique_ptr< Vtk::VtkData> GetVtkData(const IndexTree<3>& tree);
}
