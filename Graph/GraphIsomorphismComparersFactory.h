#pragma once

#include "GraphIsomorphismComparers.h"

namespace GraphIsomorphism
{

class ComparersFactory : public ICompareFactory

{
  public:
    ComparersFactory(); // Uses all known factories
    explicit ComparersFactory(std::vector<ICompareFactory *>);

    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) override;
    std::unique_ptr<Comparers> create(const Graph::IGraphUs &);
    static std::vector<ICompareFactory *> getAllSimpleFactories();
    std::vector<std::unique_ptr<ICompare>> getAllComparers(const Graph::IGraphUs &);

  private:
    std::vector<ICompareFactory *> m_factories;
};

} // namespace GraphIsomorphism
