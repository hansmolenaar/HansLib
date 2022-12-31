#include "HierBasisFunction.h"
#include "IHierBasisFunction1D.h"

#include "Utilities/Defines.h"
#include "Utilities/Assert.h"
#include "Utilities/MultiIndex.h"

namespace
{
   HierMultiIndex CreateMultiIndex(const std::vector<const IHierBasisFunction1D*>& fies)
   {
      std::vector<HierLevelIndex> multiIndex;
      str::transform(fies, std::back_inserter(multiIndex), [](const IHierBasisFunction1D* fie) {return fie->getLevelIndex(); });
      return HierMultiIndex(std::move(multiIndex));
   }
}

HierBasisFunction::HierBasisFunction(const std::vector<const IHierBasisFunction1D*>& fies) :
   m_basisFunctions(fies), m_multiIndex(CreateMultiIndex(m_basisFunctions))
{
}

const HierMultiIndex& HierBasisFunction::getMultiIndex() const
{
   return m_multiIndex;
}

const std::vector<const IHierBasisFunction1D*>& HierBasisFunction::getBasisFunctions1D() const
{
   return m_basisFunctions;
}

double HierBasisFunction::operator()(std::span<const double> x) const
{
   Utilities::Assert(m_basisFunctions.size() == x.size());
   double result = 1.0;
   for (size_t n = 0; n < m_basisFunctions.size(); ++n)
   {
      result *= m_basisFunctions.at(n)->Evaluate(x[n]);
   }
   return result;
}


HierBasisFunction_Factory::HierBasisFunction_Factory(size_t dimension, const IHierBasisFunction1D_Factory* factory) :
   m_dimension(dimension), m_factory1D(factory)
{}

bool HierBasisFunction_Factory::canBeRefined(const HierMultiIndex& hmi) const
{
   return str::all_of(hmi.get(), [this](const auto& li) { return m_factory1D->canBeRefined(li); });
}

std::unique_ptr<IHierBasisFunction> HierBasisFunction_Factory::create(const HierMultiIndex& hmi) const
{
   Utilities::Assert(m_dimension == hmi.getDimension());
   std::vector<const IHierBasisFunction1D*> basisFunctions;
   str::transform(hmi.get(), std::back_inserter(basisFunctions), [this](const auto& li) {return m_factory1D->get(li); });
   return std::make_unique<HierBasisFunction>(basisFunctions);
}

std::vector<HierMultiIndex> HierBasisFunction_Factory::getLowestLevel() const
{
   const auto lowest1D = m_factory1D->getLowestLevel();
   const auto indexer = MultiIndex<size_t>::Create(std::vector<size_t>(m_dimension, lowest1D.size()));
   std::vector<HierMultiIndex> result;
   result.reserve(indexer.getFlatSize());
   for (size_t n = 0; n < indexer.getFlatSize(); ++n)
   {
      const auto multiplet = indexer.toMultiplet(n);
      std::vector<HierLevelIndex> levelsIndices;
      str::transform(multiplet, std::back_inserter(levelsIndices), [&lowest1D](size_t n) {return lowest1D.at(n); });
      result.emplace_back(std::move(levelsIndices));
   }
   return result;
}