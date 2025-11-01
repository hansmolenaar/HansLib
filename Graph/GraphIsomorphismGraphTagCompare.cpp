#include "GraphIsomorphismGraphTagCompare.h"
#include "Defines.h"

using namespace GraphIsomorphism;

GraphTagCompare::GraphTagCompare(const TaggerNumbersFactory &numbersFactory,
                                 const std::vector<const IGraphTaggerFactory *> &others)
    : m_numbersFactory(numbersFactory)
{
    str::copy_if(others, std::back_inserter(m_others), [](const auto *ptr) { return ptr != nullptr; });
}

Status GraphTagCompare::compare(const Graph::IGraphUS &graph0, const Graph::IGraphUS &graph1) const
{

    return {};
}
