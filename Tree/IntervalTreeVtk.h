#pragma once

#include "IntervalTree.h"
#include "VtkData.h"

#include <memory>

namespace IntervalTree
{
template <size_t N> std::unique_ptr<Vtk::VtkData> GetVtkData(const IndexTree<N> &tree, const Vtk::Name &name);

template <> std::unique_ptr<Vtk::VtkData> GetVtkData(const IndexTree<1> &tree, const Vtk::Name &name);

template <> std::unique_ptr<Vtk::VtkData> GetVtkData(const IndexTree<2> &tree, const Vtk::Name &name);

template <> std::unique_ptr<Vtk::VtkData> GetVtkData(const IndexTree<3> &tree, const Vtk::Name &name);
} // namespace IntervalTree
