#pragma once

#include "MyException.h"

#include <algorithm>
#include <numeric>
#include <type_traits>
#include <vector>

namespace Functors
{
template <typename T> struct IsEqualTo
{
    T Expect;
    bool operator()(const T &actual) const
    {
        return actual == Expect;
    }
};

template <typename T> struct IsInIncludeBounds
{
    T LowerBound;
    T UpperBound;
    bool operator()(const T &val) const
    {
        return (val >= LowerBound) && (val <= UpperBound);
    }
};

template <typename TDerived> struct IsOfDerivedType
{
    template <typename TBase> bool operator()(const TBase *base) const
    {
        return dynamic_cast<const TDerived *>(base) != nullptr;
    }

  private:
};

struct AreClose
{
    double RelTolerance = 1.0e-12;
    double AbsTolerance = 1.0e-100;
    mutable double Fraction = std::numeric_limits<double>::quiet_NaN();
    bool operator()(double x, double y) const
    {
        const double maxabs = std::max(std::abs(x), std::abs(y));
        if (maxabs < AbsTolerance)
            return true;
        Fraction = std::abs(x - y) / maxabs;
        return std::abs(x - y) <= maxabs * RelTolerance;
    }
};

struct VectorDoubleLess
{
    AreClose AreCloseValue;

    bool operator()(const std::vector<double> &a, const std::vector<double> &b) const
    {
        if (a.size() != b.size())
            throw MyException("VectorDoubleLess dimension mismatch");
        for (size_t n = 0; n < a.size(); ++n)
        {
            if (AreCloseValue(a[n], b[n]))
                continue;
            return a[n] < b[n];
        }

        // Are equal
        return false;
    }
};

struct PointerIsNull
{
    bool operator()(const void *ptr) const
    {
        return ptr == nullptr;
    }
};

struct PointerIsNotNull
{
    bool operator()(const void *ptr) const
    {
        return ptr != nullptr;
    }
};
} // namespace Functors
