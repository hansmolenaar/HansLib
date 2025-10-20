#pragma once
#include <array>
#include <cstddef>

namespace IntervalTree
{
struct AdjacentDirection
{
    int Direction;
    bool UsePositiveDirection;
    auto operator<=>(const AdjacentDirection &) const = default;
};

template <size_t N> const std::array<AdjacentDirection, 2 * N> &GetAdjacentNeighbors();

template <> const std::array<AdjacentDirection, 2> &GetAdjacentNeighbors<1>();

template <> const std::array<AdjacentDirection, 4> &GetAdjacentNeighbors<2>();

template <> const std::array<AdjacentDirection, 6> &GetAdjacentNeighbors<3>();

const std::array<AdjacentDirection, 2> &GetAdjacentNeighbors1();
const std::array<AdjacentDirection, 4> &GetAdjacentNeighbors2();
const std::array<AdjacentDirection, 6> &GetAdjacentNeighbors3();
} // namespace IntervalTree
