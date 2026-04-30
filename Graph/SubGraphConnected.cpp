#include "SubGraphConnected.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Graph;
using namespace Utilities;

SubGraphConnected::SubGraphConnected(const IGraphUs &master, const std::set<Vertex> &subSet) : SubGraph(master, subSet)
{
    MyAssert(isConnected());
}
