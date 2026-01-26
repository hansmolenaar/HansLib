#include "GraphIsomorphismGraphComparers.h"
#include "Iota.h"
#include "MyAssert.h"

using namespace Graph;
using namespace Utilities;
using namespace GraphIsomorphism;

GraphComparers::GraphComparers(std::vector<const IGraphCompare *> comparers) : m_graphComparers(std::move(comparers))
{
}

const IGraphUs &GraphComparers::getGraph() const
{
    return m_graphComparers.front()->getGraph();
}

std::weak_ordering GraphComparers::compareCharacteristics(const IGraphCompare &other) const
{
    const auto &compararers0 = m_graphComparers;
    const auto &compararers1 = dynamic_cast<const GraphComparers &>(other).m_graphComparers;
    MyAssert(compararers0.size() == compararers1.size());
    std::weak_ordering result = std::weak_ordering::equivalent;
    for (size_t n : Iota::GetRange(compararers0.size()))
    {
        result = compararers0.at(n)->compareCharacteristics(*compararers1.at(n));
        if (result != std::weak_ordering::equivalent)
        {
            return result;
        }
    }
    return result;
}
