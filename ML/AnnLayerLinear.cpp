#include "AnnLayerLinear.h"

namespace
{
   double function(double x)
   {
      return x;
   }

   double derivative(double)
   {
      return 1;
   }
}

ML::AnnLayerLinear::AnnLayerLinear(size_t dim) : ML::AnnLayerUniform(dim, function, derivative)
{
}