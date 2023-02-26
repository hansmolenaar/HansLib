#include "CompassSearch.h"
#include "MyException.h"
#include "BoundsCheck.h"

CompassSearch::CompassSearch(std::unique_ptr<IMultiVariableFunctionEvaluate>&& fie, std::span<const double> init, double del) :
   m_objectiveFunction(std::move(fie)), m_currentArg(init.begin(), init.end()), m_delta(del)
{
   BoundsCheck<double>::CreateIsPositive()(del);
   m_currentEval = (*m_objectiveFunction)(m_currentArg);
}

double CompassSearch::getEvalMin()
{
   return m_currentEval;
}

std::vector<double> CompassSearch::getArgMin() const
{
   return m_currentArg;
}

bool CompassSearch::iterate()
{
   constexpr int maxLoop = 64;
   ++m_numIterations;

   for (int loop = 0; loop < maxLoop; ++loop)
   {
      for (size_t d = 0; d < m_currentArg.size(); ++d)
      {
         for (int n = 1; n > -2; n -= 2)
         {
            const double deltaSigned = n * m_delta;
            std::vector<double> trial = m_currentArg;
            trial.at(d) += deltaSigned;
            const double eval = (*m_objectiveFunction)(trial);
            if (eval < m_currentEval)
            {
               m_currentArg = trial;
               m_currentEval = eval;
               return true;
            }
         }
      }

      // nothing found? half step size
      m_delta *= 0.5;
   }
   return false;
}

int CompassSearch::getNumIterations() const
{
   return m_numIterations;
}
