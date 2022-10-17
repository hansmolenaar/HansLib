#pragma once

#include <span>
#include "Groups/IFinitePointGroup.h"

namespace IFinitePointGroupTest
{
   void TestPointGroup(const IFinitePointGroup& group, std::span<const double>);
};








