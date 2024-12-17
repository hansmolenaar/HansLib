#pragma once

#include <vector>
#include <span>

class Permutation
{
public:
   static Permutation CreateTrivial(int);
   static Permutation Create(std::span<const int>);
   static Permutation CreateFromCycle(int, std::span<const int>);
   static Permutation CreateFromDisjunctCycles(int, std::initializer_list<std::initializer_list<int>> cyvles );

   template<typename InputIt, typename OutputIt>
   void apply(InputIt first, OutputIt d_first) const;

   int operator()(int) const;
   int getCardinality() const;
  
private:
   explicit Permutation(std::vector<int>&&);
   std::vector<int> m_permut;
};

template<typename InputIt, typename OutputIt>
void Permutation::apply(InputIt first, OutputIt d_first) const
{
   for (auto n : m_permut)
   {
      d_first[n] = *first;
      ++first;
   }
}