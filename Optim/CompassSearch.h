#pragma once

#include "IIterativeMinimization.h"
#include "IMultiVariableFunctionEvaluate.h"
#include <memory>

class IMultiVariableFunctionEvaluate;

class CompassSearch : public IIterativeMinimization
{
public:
   CompassSearch(std::shared_ptr<IMultiVariableFunctionEvaluate> fie, std::vector<double> init,double del);

   IterativeMinimizationStep iterate() override;
   const IMultiVariableFunctionEvaluate& getObjectiveFunctions() const override;

private:
   std::shared_ptr<IMultiVariableFunctionEvaluate> m_objectiveFunction;
   std::vector<double> m_currentArg;
   double  m_currentEval = 0;
   double m_delta;
};