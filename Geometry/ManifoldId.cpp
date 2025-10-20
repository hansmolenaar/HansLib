
#include "ManifoldId.h"

using namespace Geometry;
using namespace Topology;

ManifoldId::ManifoldId(Topology::TopologyDimension dimension, const std::string &name)
    : m_dimension(dimension), m_name(name)
{
}

Topology::TopologyDimension ManifoldId::getTopologyDimension() const
{
    return m_dimension;
}

const std::string &ManifoldId::getName() const
{
    return m_name;
}
