#pragma once

#include "TopologyDefines.h"
#include <string>

namespace Geometry
{

class IManifoldId
{
  public:
    virtual ~IManifoldId() = default;

    virtual Topology::TopologyDimension getTopologyDimension() const = 0;
    virtual const std::string &getName() const = 0;

    friend std::strong_ordering operator<=>(const IManifoldId &lhs, const IManifoldId &rhs)
    {
        auto result = lhs.getTopologyDimension() <=> rhs.getTopologyDimension();
        if (result == std::strong_ordering::equal)
        {
            result = lhs.getName() <=> rhs.getName();
        }
        return result;
    }

    friend bool operator==(const IManifoldId &lhs, const IManifoldId &rhs)
    {
        if (lhs.getTopologyDimension() != rhs.getTopologyDimension())
        {
            return false;
        }

        return lhs.getName() == rhs.getName();
    }
};

} // namespace Geometry
