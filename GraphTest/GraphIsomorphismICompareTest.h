#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{

void CheckList(GraphIsomorphism::ICompareFactory &, const std::vector<std::string> &, GraphIsomorphism::Tag);

} // namespace GraphTest
