#pragma once

#include <vector>
#include <span>

class Permutation
{
public:
   static Permutation CreateTrivial(int);
   static Permutation Create(std::span<const int>);
   static Permutation CreateFromCycle(int, std::span<const int>);

   int operator()(int) const;
   int getCardinality() const;
  
private:
   explicit Permutation(std::vector<int>&&);
   std::vector<int> m_permut;
};