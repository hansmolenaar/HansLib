#pragma once

#include "INodeRefinePredicateFactory.h"

#include <set>

class NodeRefinePredicateAdaptive : public INodeRefinePredicate
{
  public:
    // No directional dependence yet
    explicit NodeRefinePredicateAdaptive(std::set<const HierTreeNode *> toRefine);
    bool operator()(const HierTreeNode *htn, int dir) const override;

  private:
    std::set<const HierTreeNode *> m_toRefine;
};

class NodeRefinePredicateFactoryAdaptive : public INodeRefinePredicateFactory
{
  public:
    NodeRefinePredicateFactoryAdaptive(double alpha, double fraction);
    std::unique_ptr<INodeRefinePredicate> create(const HierApproximation &approximation) override;

  private:
    double m_alpha;
    double m_fraction;
};