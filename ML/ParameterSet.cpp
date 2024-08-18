#include "ParameterSet.h"


void ML::ParameterSet::add(std::initializer_list<double> parametersForTransformation)
{
   m_allParameters.emplace_back(std::vector<double>(parametersForTransformation));
}


void ML::ParameterSet::add(std::vector<double> parametersForTransformation)
{
   m_allParameters.emplace_back(std::move(parametersForTransformation));
}

size_t ML::ParameterSet::getNumLayers() const
{
   return m_allParameters.size();
}

std::span<const double> ML::ParameterSet::at(size_t n) const
{
   return m_allParameters.at(n);
}

std::span< double> ML::ParameterSet::getModifiable(size_t n)
{
   return m_allParameters.at(n);
}

ML::ParameterSet ML::ParameterSet::CreateUsingDimensions(const ML::ParameterSet& parameterSet)
{
   ParameterSet result;
   for (auto p : parameterSet.m_allParameters)
   {
      result.add(p);
   }
   return result;
}