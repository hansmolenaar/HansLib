#include "AnnArray.h"

#include "MyAssert.h"

ML::AnnArray::AnnArray(std::span<const size_t> dimensions) : m_offset(dimensions.size() + 1)
{
    size_t totalSize = 0;
    m_offset[0] = 0;
    for (size_t n = 0; n < dimensions.size(); ++n)
    {
        m_offset[n + 1] = m_offset[n] + dimensions[n];
        totalSize += dimensions[n];
    }
    m_values.resize(totalSize);
}

size_t ML::AnnArray::getNumLayers() const
{
    return m_offset.size() - 1;
}

size_t ML::AnnArray::getLayerSizeAt(size_t n) const
{
    return m_offset.at(n + 1) - m_offset.at(n);
}

std::span<const double> ML::AnnArray::getValues(size_t n) const
{
    return std::span<const double>(m_values.begin() + m_offset.at(n), getLayerSizeAt(n));
}

std::span<double> ML::AnnArray::getValuesMutable(size_t n)
{
    return std::span<double>(m_values.begin() + m_offset.at(n), getLayerSizeAt(n));
}