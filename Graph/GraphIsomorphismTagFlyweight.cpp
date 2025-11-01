#include "GraphIsomorphismTagFlyweight.h"

using namespace GraphIsomorphism;

namespace
{
std::map<TagEntry, std::unique_ptr<Tag>> Tags;
};

const Tag &TagFlyweight::getSingleEntryTag(TagEntry entry)
{
    if (!Tags.contains(entry))
    {
        Tags[entry] = std::make_unique<Tag>(1, entry);
    }
    return *Tags.at(entry);
};
