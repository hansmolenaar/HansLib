#pragma once

#include <gtest/gtest.h>
#include "Interpolation/IHierBasisFunction1D.h"
#include <numeric>

inline void IHierBasisFunction1DTest_Interface(const IHierBasisFunction1D& fie)
{
   constexpr double epsilon = 1.0e-12;
   ASSERT_FALSE(fie.HasDerivative());
   ASSERT_ANY_THROW(fie.Derivative(0.0));
   const auto support = fie.getSupport();
   ASSERT_LT(support.getLower(), support.getUpper());
   ASSERT_GE(support.getLower(), 0);
   ASSERT_LE(support.getUpper(), 1.0);
   const double fieL = fie(support.getLower());
   const double fieH = fie(support.getUpper());
   if (std::abs(fieL) + std::abs(fieH) < epsilon)
   {
      const double fieM = fie(std::midpoint(support.getLower(), support.getUpper()));
      ASSERT_NEAR(fieM, 1.0, epsilon);
   }
}
