#include "CompassSearch.h"
#include "IterativeMinimizationStep.h"
#include "MyException.h"
#include "BoundsCheck.h"

CompassSearch::CompassSearch(std::shared_ptr<IMultiVariableFunctionEvaluate> fie, std::vector<double> init, double del) :
   m_objectiveFunction(std::move(fie)), m_currentArg(std::move(init)), m_delta(del)
{
   BoundsCheck<double>::CreateIsPositive()(del);
   m_currentEval = (*m_objectiveFunction)(m_currentArg);
}

IterativeMinimizationStep CompassSearch::iterate()
{
   constexpr int maxLoop = 64;
   IterativeMinimizationStep result;

   for (int loop = 0; loop < maxLoop; ++loop)
   {
      for (size_t d = 0; d < m_currentArg.size(); ++d)
      {
         for (int n = 1; n > -2; n -= 2)
         {
            const double deltaSigned = n * m_delta;
            result.ArgMinimum = m_currentArg;
            result.ArgMinimum.at(d) += deltaSigned;
            result.EvalMinimum = (*m_objectiveFunction)(result.ArgMinimum);
            if (result.EvalMinimum < m_currentEval)
            {
               m_currentArg = result.ArgMinimum;
               m_currentEval = result.EvalMinimum;
               result.Status = StepSucces;
               return result;
            }
         }
      }

      // nothing found? half step size
      m_delta *= 0.5;
   }

   result.Status = StepStuck;
   return result;
}


const IMultiVariableFunctionEvaluate& CompassSearch::getObjectiveFunction() const
{
   return *m_objectiveFunction;
}