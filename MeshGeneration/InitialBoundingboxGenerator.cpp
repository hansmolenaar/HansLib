#include "InitialBoundingboxGenerator.h"
#include "Defines.h"
#include "MyException.h"

template class InitialBoundingboxGenerator<1>;
template class InitialBoundingboxGenerator<2>;
template class InitialBoundingboxGenerator<3>;

template <size_t N>
std::unique_ptr<IInitialBoundingboxGenerator<N>> InitialBoundingboxGenerator<N>::Create(double factor)
{
    if (factor < 1.0)
        throw MyException("InitialboundingboxGenerator<N>::Create factor should at least be 1");
    return std::unique_ptr<InitialBoundingboxGenerator<N>>(new InitialBoundingboxGenerator<N>(factor));
}

template <size_t N> InitialBoundingboxGenerator<N>::InitialBoundingboxGenerator(double factor) : m_factor(factor)
{
}

template <size_t N>
BoundingBox<double, N> InitialBoundingboxGenerator<N>::generate(
    const Geometry::IGeometryRegion<double, N> &region) const
{
    const auto bb = region.getBoundingBox();
    const auto &intervals = bb.getIntervals();
    std::array<double, N> center;
    str::transform(intervals, center.begin(), [](const Interval<double> &intv) { return intv.interpolate(0.5); });
    std::array<double, N> sizes;
    str::transform(intervals, sizes.begin(), [](const Interval<double> &intv) { return intv.getMeasure(); });
    const double maxSize = *str::max_element(sizes);

    std::array<double, N> lwr;
    std::array<double, N> upr;
    str::transform(center, lwr.begin(), [maxSize, this](double c) { return c - 0.5 * m_factor * maxSize; });
    str::transform(center, upr.begin(), [maxSize, this](double c) { return c + 0.5 * m_factor * maxSize; });

    return BoundingBox<double, N>::CreateFrom2Points(lwr, upr);
}
