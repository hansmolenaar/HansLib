#include "ParameterSet.h"


void ML::ParameterSet::add(std::initializer_list<double> parametersForTransformation)
{
   m_allParameters.emplace_back(std::vector<double>(parametersForTransformation));
}

std::span<const double> ML::ParameterSet::at(size_t n) const
{
   return m_allParameters.at(n);
}