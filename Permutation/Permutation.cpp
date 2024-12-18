#include "Permutation.h"
#include "MyAssert.h"
#include "PermutationUtils.h"
#include "BoundsCheck.h"
#include "Defines.h"


Permutation::Permutation(std::vector<int>&& permut) :
   m_permut(std::move(permut))
{
   if (!PermutationUtils::IsPermutation(m_permut))
   {
      throw MyException("Permutation is not a permutation!!");
   }
}

Permutation Permutation::CreateTrivial(int cardinality)
{
   Utilities::MyAssert(cardinality > 0);
   std::vector<int> permut(cardinality);
   for (int n = 0; n < cardinality; ++n) permut[n] = n;
   return Permutation(std::move(permut));
}

int Permutation::operator()(int n) const
{
   return m_permut.at(n);
}

int Permutation::getCardinality() const
{
   return static_cast<int>(m_permut.size());
}

Permutation Permutation::Create(std::span<const int> permutSpan)
{
   std::vector<int> permut(permutSpan.begin(), permutSpan.end());
   Utilities::MyAssert(PermutationUtils::IsPermutation(permut));
   return Permutation(std::move(permut));
}

Permutation Permutation::CreateFromCycle(int cardinality, std::span<const int> cycle)
{
   Utilities::MyAssert(cardinality > 0);
   const auto bounds = BoundsCheck<int>::Create(0, cardinality - 1);
   Utilities::MyAssert(str::all_of(cycle, bounds));
   std::vector<int> permut(cardinality, -1);
   for (size_t n = 0; n < cycle.size(); ++n)
   {
      permut.at(cycle[n]) = cycle[(n + 1) % cycle.size()];
   }
   for (size_t n = 0; n < permut.size(); ++n)
   {
      if (permut.at(n) == -1) permut.at(n) = static_cast<int>(n);
   }
   return Permutation::Create(permut);
}

Permutation Permutation::CreateFromDisjunctCycles(int cardinality, std::initializer_list<std::initializer_list<int>> cycles)
{
   std::vector<int> perm(cardinality, -1);
   for (const auto& cycle : cycles)
   {
      const auto minmax = str::minmax_element(cycle);
      if (*minmax.min < 0) throw MyException("CreateFromDisjunctCycles min element out of range: " + std::to_string(*minmax.min));
      if (*minmax.max >= cardinality) throw MyException("CreateFromDisjunctCycles max element out of range: " + std::to_string(*minmax.max));
      int prev = -1;
      for (int c : cycle)
      {
         if (prev != -1)
         {
            if (perm[prev] != -1) throw MyException("Permutation::CreateFromDisjunctCycles duplicate entry!");
            perm[prev] = c;
         }
         prev = c;
      }
      if (perm[prev] != -1) throw MyException("Permutation::CreateFromDisjunctCycles duplicate entry!");
      perm[prev] = *cycle.begin();
   }
   for (int n = 0; n < cardinality; ++n)
   {
      if (perm[n] == -1)
      {
         perm[n] = n;
      }
   }
   if (!PermutationUtils::IsPermutation(perm))
   {
      throw MyException("Permutation::CreateFromDisjunctCycles not a permutation??");
   }
   return Permutation(std::move(perm));
}


Permutation operator*(const Permutation& perm1, const Permutation& perm0)
{
   Utilities::MyAssert(perm1.getCardinality() == perm0.getCardinality());
   const auto siz = perm0.getCardinality();
   std::vector<int> permut(siz, -1);
   for (auto n = 0; n < siz; ++n)
   {
      permut.at(n) = perm1(perm0(n));
   }
   return Permutation::Create(permut);
}

bool operator==(const Permutation& perm0, const Permutation& perm1)
{
   return str::equal(perm0.m_permut, perm1.m_permut);
}

std::strong_ordering operator<=>(const Permutation& perm0, const Permutation& perm1)
{
   return perm0.m_permut <=> perm1.m_permut;
}