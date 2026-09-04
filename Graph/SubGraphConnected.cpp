#include "SubGraphConnected.h"
#include "MyAssert.h"

using namespace Graph;
using namespace Utilities;

SubGraphConnected::SubGraphConnected(const IGraphUs &master, const std::set<Vertex> &subSet) : SubGraph(master, subSet)
{
   MyAssert(isConnected());
}
