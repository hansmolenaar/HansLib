#pragma once

#include "MyAssert.h"

template <typename T> class BoundsCheck
{
  public:
    BoundsCheck() = default; // anything goes
    static BoundsCheck<T> Create(T, T);
    static BoundsCheck<T> CreateLowerBound(T);
    static BoundsCheck<T> CreateUpperBound(T);
    static BoundsCheck<T> CreateIsPositive();    // > 0
    static BoundsCheck<T> CreateIsNonNegative(); // >= 0
    bool operator()(const T &) const;
    void check(const T &) const;

  private:
    BoundsCheck(bool, T, bool, T);
    bool m_hasLb = false;
    bool m_hasUb = false;
    T m_lb = 0;
    T m_ub = 0;
};

template <typename T> BoundsCheck<T> BoundsCheck<T>::Create(T lb, T ub)
{
    Utilities::MyAssert(lb <= ub);
    return BoundsCheck(true, lb, true, ub);
}

template <typename T> BoundsCheck<T> BoundsCheck<T>::CreateLowerBound(T bound)
{
    return BoundsCheck(true, bound, false, bound);
}

template <typename T> BoundsCheck<T> BoundsCheck<T>::CreateUpperBound(T bound)
{
    return BoundsCheck(false, bound, true, bound);
}

template <typename T> BoundsCheck<T> BoundsCheck<T>::CreateIsPositive()
{
    return CreateLowerBound(static_cast<T>(1));
}

template <typename T> BoundsCheck<T> BoundsCheck<T>::CreateIsNonNegative()
{
    return CreateLowerBound(static_cast<T>(0));
}

template <typename T> bool BoundsCheck<T>::operator()(const T &val) const
{
    if (m_hasLb && val < m_lb)
        return false;
    if (m_hasUb && val > m_ub)
        return false;
    return true;
}

template <typename T>
BoundsCheck<T>::BoundsCheck(bool hasLb, T lb, bool hasUb, T ub)
    : m_hasLb(hasLb), m_hasUb(hasUb), m_lb(std::move(lb)), m_ub(std::move(ub))
{
}

template <typename T> void BoundsCheck<T>::check(const T &value) const
{
    Utilities::MyAssert((*this)(value), "Bounds violation");
}