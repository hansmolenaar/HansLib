#pragma once

#include "IPointTransformation.h"
#include "IFinitePointGroupAction.h"

template<int N>
class PointTransformationFromGroupAction : public IPointTransformation<double, N>
{
public:
   PointTransformationFromGroupAction(const IFinitePointGroupAction<N>& a, int e) : m_action(a), m_element(e) {};
   Point<double, N> operator()(const Point<double, N>&) const override;
private:
   const IFinitePointGroupAction<N>& m_action;
   int m_element;
};


template<int N>
Point<double, N> PointTransformationFromGroupAction<N>::operator()(const Point<double, N>& p) const
{
   return m_action(m_element, p);
}