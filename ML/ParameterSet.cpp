#include "ParameterSet.h"


void ML::ParameterSet::add(std::initializer_list<double> parametersForTransformation)
{
   m_allParameters.emplace_back(std::vector<double>(parametersForTransformation));
}


void ML::ParameterSet::add(std::vector<double> parametersForTransformation)
{
   m_allParameters.emplace_back(std::move(parametersForTransformation));
}

void ML::ParameterSet::add(std::span<const double> parametersForTransformation)
{
   add(std::vector<double>(parametersForTransformation.begin(), parametersForTransformation.end()));
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

ML::ParameterSet ML::ParameterSet::CreateUsingDimensions(const ML::IParameterSet& parameterSet)
{
   ParameterSet result;
   for (size_t layer = 0; layer < parameterSet.getNumLayers(); ++layer)
   {
      result.m_allParameters.emplace_back(std::vector<double>(parameterSet.at(layer).size()));
   }
   return result;
}

ML::ParameterSet ML::ParameterSet::CopyFrom(const ML::IParameterSet& parameterSet)
{
   ParameterSet result;
   const size_t numLayers = parameterSet.getNumLayers();
   for (size_t layer = 0; layer < numLayers; ++layer)
   {
      result.add(parameterSet.at(layer));
   }
   return result;
}