#pragma once

#include <vector>

class Permutation
{
public:
   static Permutation CreateTrivial(int);
 
   int operator()(int) const;
   int getCardinality() const;
private:
   Permutation(std::vector<int>&&);
   std::vector<int> m_permut;
};