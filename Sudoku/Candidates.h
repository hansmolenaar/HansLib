#pragma once

#include "SudokuDefines.h"
#include <boost/container/static_vector.hpp>

#include <initializer_list>
#include <iterator>
#include <optional>

namespace Sudoku
{

class CandidatesIterator
{
 public:
   typedef boost::container::static_vector<Value, NumValue>::const_iterator CIter;

   CandidatesIterator() = default;
   CandidatesIterator(CIter itr, CIter end) : m_ptr(itr), m_end(end)
   {
      skipInvalid();
   };

   const Value &operator*() const
   {
      return *m_ptr;
   }
   using iterator_category = std::forward_iterator_tag;
   using value_type = Value;
   using difference_type = std::ptrdiff_t;
   using pointer = Value *;
   using reference = Value &;

   CandidatesIterator operator++(int)
   {
      auto temp(*this);
      ++m_ptr;
      skipInvalid();
      return temp;
   }

   CandidatesIterator &operator++()
   {
      ++m_ptr;
      skipInvalid();
      return (*this);
   }

   bool operator!=(const CandidatesIterator &other) const
   {
      return (m_ptr != other.m_ptr);
   };

   bool operator==(const CandidatesIterator &other) const = default;

 private:
   void skipInvalid()
   {
      while (m_ptr != m_end && *m_ptr == ValueUndefined)
      {
         ++m_ptr;
      };
   };

   CIter m_ptr;
   CIter m_end;
};

class Candidates
{
 public:
   bool setOnly(std::initializer_list<Value>);
   bool removeCandidate(Value);
   bool isSolved() const;
   Index size() const;
   std::optional<Value> tryGetSolvedValue() const;
   bool contains(Value) const;
   bool contains(std::span<const Value>) const;

   auto operator<=>(const Sudoku::Candidates &) const = default;

   static std::array<std::string, NumBoxIndex> ToStringsDifferences(const Candidates &);
   static std::array<std::string, NumBoxIndex> ToStrings(const Candidates &);

   CandidatesIterator begin() const
   {
      return cbegin();
   }

   CandidatesIterator end() const
   {
      return cend();
   }

   CandidatesIterator cbegin() const
   {
      return CandidatesIterator(m_candidates.begin(), m_candidates.end());
   }

   CandidatesIterator cend() const
   {
      return CandidatesIterator(m_candidates.end(), m_candidates.end());
   }

 private:
   boost::container::static_vector<Value, NumIndex> m_candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};
} // namespace Sudoku
