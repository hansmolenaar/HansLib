#pragma once

#include "IAnnLayer.h"

namespace ML
{
   using RealFunctionPtr = double (*)(double);

   // Artificial Neural Network, layer
   class AnnLayerUniform : public ML::IAnnLayer
   {
   public:
      AnnLayerUniform(size_t dim, RealFunctionPtr function, RealFunctionPtr derivative);
      size_t getNumberOfNeurons() const override;
      void setActivatorFunction(std::span<const double> weightedAverage, std::span<double> activatorValues) const override;
      void setActivatorFunctionDeriv(std::span<const double> weightedAverage, std::span<double> activatorDerivValues) const override;
   private:
      size_t m_size;
      RealFunctionPtr m_function;
      RealFunctionPtr m_derivative;
   };
}
