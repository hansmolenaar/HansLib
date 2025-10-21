#pragma once

#include "IFiniteGroupUtils.h"
#include "IFinitePointGroupAction.h"
#include "PointClose.h"

#include <numbers>

template <size_t N> Point<double, N> GetRandomPoint()
{
    static std::array<double, 4> values{std::numbers::pi, std::numbers::e, std::numbers::ln10, std::numbers::sqrt2};
    std::array<double, N> data;
    static_assert(N < values.size());
    std::copy_n(values.begin(), N, data.begin());

    return Point<double, N>(data);
}

template <size_t N>
void Test_IFinitePointGroupAction(const IFinitePointGroupAction<N> &groupAction, const Point<double, N> &pointInSet)
{
    const PointClose<double, N> areClose;
    const auto &group = groupAction.getGroup();
    const auto groupElements = IFiniteGroupUtils::GetElements(group);

    // Origin must be invariant
    const Point<double, N> nul(std::array<double, N>{});
    for (auto elm : groupElements)
    {
        const auto retval = groupAction(elm, nul);
        ASSERT_TRUE(areClose(nul, retval));
    }

    // Off by one
    ASSERT_ANY_THROW(groupAction(group.getOrder(), nul));

    // Identity
    const auto id = group.getIdentity();
    ASSERT_TRUE(areClose(pointInSet, groupAction(id, pointInSet)));

    // Compatibility
    for (auto elm1 : groupElements)
    {
        for (auto elm2 : groupElements)
        {
            const auto elm3 = group(elm1, elm2);
            const auto point3 = groupAction(elm3, pointInSet);

            const auto point2 = groupAction(elm2, pointInSet);
            const auto point12 = groupAction(elm1, point2);
            ASSERT_TRUE(areClose(point12, point3));
        }
    }
}
