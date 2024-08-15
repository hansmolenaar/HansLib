#pragma once

#include <vector>
#include <span>

namespace ML
{
   class AnnArray
   {
   public:
      explicit AnnArray(std::span<const size_t> dimensions);
      std::span< double> modifyValuesAt(size_t n);
      std::span<const double> getValuesAt(size_t n) const;
      size_t getNumLayers() const;
   private:
      size_t getLayerSizeAt(size_t n) const;
      std::vector<double>  m_values;
      std::vector<size_t>  m_offset;
   };
}
