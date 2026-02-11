#pragma once

#include "GraphIsomorphismComparers.h"

namespace GraphIsomorphism
{

class ComparersFactory : public IGraphCompareFactory

{
  public:
    ComparersFactory(); // Uses all known factories
    explicit ComparersFactory(std::vector<const ICompareFactory *>);

    std::unique_ptr<IGraphCompare> createGraphCompare(const Graph::IGraphUs &) const override;
    std::unique_ptr<Comparers> create(const Graph::IGraphUs &) const;
    static std::vector<const ICompareFactory *> getAllSimpleFactories();
    std::vector<std::unique_ptr<ICompare>> getAllComparers(const Graph::IGraphUs &) const;

  private:
    std::vector<const ICompareFactory *> m_factories;
};

} // namespace GraphIsomorphism
