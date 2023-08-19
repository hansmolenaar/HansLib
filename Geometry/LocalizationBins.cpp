#include "LocalizationBins.h"
#include "MyException.h"
#include "Defines.h"

namespace
{
   std::vector<double> RemoveTooSmallBins(const std::vector< double>& verticesIn, double minBinSize)
   {
      std::vector<double> result;
      result.reserve(verticesIn.size());
      result.push_back(verticesIn.front());
      for (size_t n = 1; n < verticesIn.size(); ++n)
      {
         if (verticesIn.at(n) > result.back() + minBinSize)
         {
            result.push_back(verticesIn.at(n));
         }
      }

      // Handle the edge case that the last bin was too small
      if (result.back() != verticesIn.back())
      {
         *result.rbegin() = verticesIn.back();
      }
      return result;
   }
}
LocalizationBins::LocalizationBins(std::vector<double> vertices) :
   m_vertices(std::move(vertices))
{
}

LocalizationBins LocalizationBins::CreateUniform(const Interval<double>& interval, int nBins)
{
   if (nBins < 1) throw MyException("LocalizationBins::CreateUniform need at least 1 bin");

   std::vector<double> vertices;
   vertices.reserve(static_cast<size_t>(nBins) + 1);

   vertices.push_back(interval.getLower());
   const double binSize = interval.getMeasure() / nBins;
   if (binSize <= 0.0) throw MyException("LocalizationBins::CreateUniform incorrect bin size");
   for (int n = 0; n < nBins - 1; ++n)
   {
      vertices.push_back(vertices.back() + binSize);
   }
   vertices.push_back(interval.getUpper());

   return LocalizationBins{ vertices };
}

LocalizationBins LocalizationBins::CreateFromValues(std::vector<double> values, bool strict, double minBinSize)
{
   if (minBinSize <= 0)  throw MyException("LocalizationBins::CreateFromValues minBinSize must be positive");
   if (values.size() < 2) throw MyException("LocalizationBins::CreateFromValues need at least 2 values");
   str::sort(values);
   if (values.back() - values.front() < minBinSize)
   {
      throw MyException("LocalizationBins::CreateFromValues value too close");
   }

   std::vector<double> vertices;
   vertices.reserve(values.size() + 2);

   if (!strict) vertices.push_back(std::numeric_limits<double>::lowest());
   vertices.push_back(values.front());

   for (size_t n = 1; n < values.size(); ++n)
   {
      vertices.push_back(0.5 * (values[n - 1] + values[n]));
   }

   vertices.push_back(values.back());
   if (!strict) vertices.push_back(std::numeric_limits<double>::max());

   return  LocalizationBins{ RemoveTooSmallBins(vertices, minBinSize) };
}

double LocalizationBins::getBinLower(size_t n) const 
{
   return m_vertices.at(n);
}

double LocalizationBins::getBinUpper(size_t n) const
{
   return m_vertices.at(n + 1);
}

size_t LocalizationBins::getNumBins() const
{
   return m_vertices.size() - 1;
}

size_t LocalizationBins::find(double x) const
{
   if (x < m_vertices.front()) throw MyException("LocalizationBins::find value below lower bound");
   const auto& ub = str::upper_bound(m_vertices, x);
   if (ub != m_vertices.end())
   {
      return std::distance(m_vertices.begin(), ub) - 1;
   }

   if (x > m_vertices.back()) throw MyException("LocalizationBins::find value above upper bound");
   // x == m_vertices.back()
   return m_vertices.size() - 2;
}