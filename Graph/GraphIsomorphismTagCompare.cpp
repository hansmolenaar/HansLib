#include "GraphIsomorphismTagCompare.h"
#include "GraphIsomorphismGrouper.h"

using namespace GraphIsomorphism;

TagCompare::Result TagCompare::operator()(std::pair<const ITagger &, const ITagger &> taggers) const
{
    std::vector<VertexPair> vertexPairs;
    const Grouper grouper1(taggers.first);
    const Grouper grouper2(taggers.second);
    const auto &tags1 = grouper1.getTags();
    const auto &tags2 = grouper2.getTags();

    if (tags1.size() != tags2.size())
    {
        return {};
    }

    for (size_t n = 0; n < tags1.size(); ++n)
    {
        if (tags1[n] != tags2[n])
        {
            return {};
        }
        const auto &members1 = grouper1.getGroupMembers(tags1[n]);
        const auto &members2 = grouper2.getGroupMembers(tags2[n]);
        if (members1.size() != members2.size())
        {
            return {};
        }
        if (members1.size() == 1)
        {
            vertexPairs.push_back({members1.front(), members2.front()});
        }
    }

    return {Result::Status::Equivalent, vertexPairs};
}
