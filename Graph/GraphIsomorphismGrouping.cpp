#include "GraphIsomorphismGrouping.h"
#include "GraphDefines.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;

template GraphIsomorphism::Grouping<Vertex> Grouping<Vertex>::Combine(const Grouping<Vertex> &grouping0,
                                                                      const Grouping<Vertex> &grouping1);

namespace
{

template <typename T> std::map<T, size_t> Invert(const Grouping<T> &grouping)
{
    const auto size = grouping.size();
    std::map<T, size_t> result;
    for (size_t g = 0UZ; g < grouping().size(); ++g)
    {
        for (Vertex vertex : grouping().at(g))
        {
            result[vertex] = g;
        }
    }

    Utilities::MyAssert(result.begin()->first == 0);
    Utilities::MyAssert(result.rbegin()->first + 1 == size);
    return result;
}

} // namespace

template <typename T>
GraphIsomorphism::Grouping<T> Grouping<T>::Combine(const Grouping<T> &grouping0, const Grouping<T> &grouping1)
{
    const auto size = grouping0.size();
    Utilities::MyAssert(size == grouping1.size());

    const auto map0 = Invert(grouping0);
    const auto map1 = Invert(grouping1);
    std::vector<T> vertices(size);
    str::iota(vertices, static_cast<T>(0));
    auto cmp = [&map0, &map1](Vertex v0, Vertex v1) {
        return std::make_pair(map0.at(v0), map1.at(v0)) < std::make_pair(map0.at(v1), map1.at(v1));
    };
    return Grouping<T>(vertices, cmp);
}
