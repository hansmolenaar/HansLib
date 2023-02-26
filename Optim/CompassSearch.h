#pragma once

#include "IIterativeMinimization.h"
#include "IMultiVariableFunctionEvaluate.h"
#include <memory>

class IMultiVariableFunctionEvaluate;

class CompassSearch : public IIterativeMinimization
{
public:
   CompassSearch(std::unique_ptr<IMultiVariableFunctionEvaluate>&& fie, std::span<const double> init,double del);

   double getEvalMin() override;
   std::vector<double> getArgMin() const override;
   bool iterate() override;
   int getNumIterations() const override;

private:
   std::unique_ptr<IMultiVariableFunctionEvaluate> m_objectiveFunction;
   std::vector<double> m_currentArg;
   double  m_currentEval = 0;
   double m_delta;
   int m_numIterations = 0;
};
