#include "GraphIsomorphismCharacteristicsComparers.h"
#include "Iota.h"
#include "MyAssert.h"

using namespace Graph;
using namespace Utilities;
using namespace GraphIsomorphism;

CharacteristicsComparers::CharacteristicsComparers(std::vector<const ICharacteristicsCompare *> comparers)
    : m_characteristicsComparers(std::move(comparers))
{
}

const IGraphUs &CharacteristicsComparers::getGraph() const
{
    return m_characteristicsComparers.front()->getGraph();
}

std::weak_ordering CharacteristicsComparers::compareCharacteristics(const ICharacteristicsCompare &other) const
{
    const auto &compararers0 = m_characteristicsComparers;
    const auto &compararers1 = dynamic_cast<const CharacteristicsComparers &>(other).m_characteristicsComparers;
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
