#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{

void CheckList(GraphIsomorphism::ICompareFactory &, const std::vector<std::string> &, GraphIsomorphism::Tag);
void CheckList(GraphIsomorphism::ICompareFactory &, const std::vector<std::unique_ptr<Graph::IGraphUs>> &,
               GraphIsomorphism::Tag);

} // namespace GraphTest
