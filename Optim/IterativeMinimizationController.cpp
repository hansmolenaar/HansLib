#include "IterativeMinimizationController.h"
#include "IIterativeMinimization.h"
#include "IIterativeMinimizationConvergenceCrit.h"

IterativeMinimizationResult IterativeMinimizationController::Iterate(IIterativeMinimization& iterator, IIterativeMinimizationConvergenceCrit& crit)
{
   std::vector<IterativeMinimizationStep> steps;

   while (true)
   {
      // enum IIterativeMinimizationStepStatus { StepSucces, StepStuck, StepDivergence };
      const auto stepResult = iterator.iterate();
      steps.push_back(stepResult);
      IterativeMinimizationResult result{ Fail, stepResult.ArgMinimum , stepResult.EvalMinimum };

      if (stepResult.Status == StepDivergence)
      {
         return result;
      }

      // enum IterativeMinimizationConvergenceStatus {Converged, NotConverged, MaxIterExceeded, Fail};
      result.Status = crit.getConvergenceStatus(steps);
      if (result.Status != NotConverged)
      {
         return result;
      }
      else if (stepResult.Status == StepStuck)
      {
         // NotConverged and StepStuck
         result.Status = MaxIterExceeded;
         return result;
      }
   }
}