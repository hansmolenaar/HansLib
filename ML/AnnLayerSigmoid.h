#pragma once

#include "AnnLayerUniform.h"

namespace ML
{
   class AnnLayerSigmoid : public ML::AnnLayerUniform
   {
   public:
      AnnLayerSigmoid(size_t dim);

      static double Sigmoid(double);
      static double SigmoidDeriv(double);

   private:
      size_t m_size;
   };
}
