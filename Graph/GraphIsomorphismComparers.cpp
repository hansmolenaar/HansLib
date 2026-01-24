#include "GraphIsomorphismComparers.h"

#include "GraphIsomorphismUtils.h"

using namespace Graph;
using namespace GraphIsomorphism;

Comparers::Comparers(std::vector<std::unique_ptr<ICompare>> &&comparers)
    : m_comparers(std::move(comparers)), m_graphComparers(selectGraphCompare(m_comparers)),
      m_vertexComparers(selectVertexCompare(m_comparers))
{
}
