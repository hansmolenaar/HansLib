#pragma once

#include "BoundsCheck.h"
#include "Defines.h"
#include "IIndexer.h"

#include <boost/container/small_vector.hpp>
#include <span>

static constexpr size_t MULTI_INDEX_MAX_SMALL_VECTOR = 6;

template <typename T> class MultiIndex : public IIndexer<T>
{
  public:
    static MultiIndex<T> Create(std::span<const T> dimensions);

    T getNumberOfIndices() const override;
    size_t toFlat(std::initializer_list<T>) const override;
    size_t getFlatSize() const override;

    T at(size_t) const;
    void toMultiplet(size_t, std::span<T>) const;
    size_t toFlat(std::span<const T>) const;

  private:
    explicit MultiIndex(boost::container::small_vector<T, MULTI_INDEX_MAX_SMALL_VECTOR> &&dimensions);
    boost::container::small_vector<T, MULTI_INDEX_MAX_SMALL_VECTOR> m_dimensions;
    size_t m_flatLength;
    boost::container::small_vector<T, MULTI_INDEX_MAX_SMALL_VECTOR> m_factors;
    BoundsCheck<T> m_checkFlat;
    boost::container::small_vector<BoundsCheck<T>, MULTI_INDEX_MAX_SMALL_VECTOR> m_checkIndex;
};
