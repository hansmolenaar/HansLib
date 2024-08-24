#include "AnnLayerUniform.h"
#include "MyAssert.h"

namespace
{
   void Eval(size_t dimExpected, std::span<const double> valuesIn, std::span<double> valuesOut, ML::RealFunctionPtr fie)
   {
      Utilities::MyAssert(valuesIn.size() == dimExpected);
      Utilities::MyAssert(valuesOut.size() == dimExpected);
      for (size_t n = 0; n < dimExpected; ++n)
      {
         valuesOut[n] = (*fie)(valuesIn[n]);
      }
   }
}

ML::AnnLayerUniform::AnnLayerUniform(size_t dim, RealFunctionPtr function, RealFunctionPtr derivative) : 
   m_size(dim), m_function(function), m_derivative(derivative)
{
}

size_t ML::AnnLayerUniform::getNumberOfNeurons() const
{
   return m_size;
}

void ML::AnnLayerUniform::setActivatorFunction(std::span<const double> weightedAverage, std::span<double> activatorValues) const
{
   Eval(m_size, weightedAverage, activatorValues, m_function);
}

void  ML::AnnLayerUniform::setActivatorFunctionDeriv(std::span<const double> weightedAverage, std::span<double> activatorDerivValues) const
{
   Eval(m_size, weightedAverage, activatorDerivValues, m_derivative);
}