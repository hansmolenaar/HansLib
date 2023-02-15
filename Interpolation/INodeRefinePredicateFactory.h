#pragma once
#include "INodeRefinePredicate.h"

#include <memory>

class HierApproximation;

class INodeRefinePredicateFactory
{
public:
   virtual ~INodeRefinePredicateFactory() noexcept = default;

   // Get cell refinement predicate given the current state of the spearse grid approximantion
   virtual std::unique_ptr<INodeRefinePredicate> create(const HierApproximation& approximation) = 0;
};
