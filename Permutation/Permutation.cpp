#include "Permutation.h"
#include "MyAssert.h"
#include "PermutationUtils.h"
#include "BoundsCheck.h"
#include "Defines.h"


Permutation::Permutation(std::vector<Entry>&& permut) :
   m_permut(std::move(permut))
{
   if (m_permut.size() >= std::numeric_limits<Entry>::max())
   {
      throw MyException("Permutation input too large");
   }
   if (!PermutationUtils::IsPermutation(m_permut))
   {
      throw MyException("Permutation is not a permutation!!");
   }
}

Permutation Permutation::CreateTrivial(Entry cardinality)
{
   std::vector<Entry> permut(cardinality);
   std::iota(permut.begin(), permut.end(), static_cast<Entry>(0));
   return Permutation(std::move(permut));
}

Permutation::Entry Permutation::operator()(Entry n) const
{
   return m_permut.at(n);
}

Permutation::Entry Permutation::getCardinality() const
{
   return static_cast<Entry>(m_permut.size());
}

Permutation Permutation::Create(std::span<const Entry> permutSpan)
{
   std::vector<Entry> permut(permutSpan.begin(), permutSpan.end());
   Utilities::MyAssert(PermutationUtils::IsPermutation(permut));
   return Permutation(std::move(permut));
}

Permutation Permutation::CreateFromCycle(Entry cardinality, std::span<const Entry> cycle)
{
   if (cardinality == 0) return CreateTrivial(0);

   const auto bounds = BoundsCheck<Entry>::Create(0, cardinality - 1);
   Utilities::MyAssert(str::all_of(cycle, bounds));
   std::vector<Entry> permut(cardinality, InvalidEntry);
   for (Entry n = 0; n < cycle.size(); ++n)
   {
      permut.at(cycle[n]) = cycle[(n + 1) % cycle.size()];
   }
   for (Entry n = 0; n < permut.size(); ++n)
   {
      if (permut.at(n) == InvalidEntry) permut.at(n) = n;
   }
   return Permutation::Create(permut);
}

Permutation Permutation::CreateFromDisjunctCycles(Entry cardinality, std::initializer_list<std::initializer_list<Entry>> cycles)
{
   std::vector<Entry> perm(cardinality, InvalidEntry);
   for (const auto& cycle : cycles)
   {
      const auto minmax = str::minmax_element(cycle);
      if (*minmax.min < 0) throw MyException("CreateFromDisjunctCycles min element out of range: " + std::to_string(*minmax.min));
      if (*minmax.max >= cardinality) throw MyException("CreateFromDisjunctCycles max element out of range: " + std::to_string(*minmax.max));
      Entry prev = InvalidEntry;
      for (int c : cycle)
      {
         if (prev != InvalidEntry)
         {
            if (perm[prev] != InvalidEntry) throw MyException("Permutation::CreateFromDisjunctCycles duplicate entry!");
            perm[prev] = c;
         }
         prev = c;
      }
      if (perm[prev] != InvalidEntry) throw MyException("Permutation::CreateFromDisjunctCycles duplicate entry!");
      perm[prev] = *cycle.begin();
   }
   for (Entry n = 0; n < cardinality; ++n)
   {
      if (perm[n] == InvalidEntry)
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

// First perm0 then perm1
Permutation operator*(const Permutation& perm1, const Permutation& perm0)
{
   Utilities::MyAssert(perm1.getCardinality() == perm0.getCardinality());
   const auto siz = perm0.getCardinality();
   std::vector<Permutation::Entry> permut(siz, Permutation::InvalidEntry);
   for (Permutation::Entry n = 0; n < siz; ++n)
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

Permutation Permutation::getInverse() const
{
   const auto order = getCardinality();
   std::vector<Entry> inverse(order);
   for (Entry n = 0; n < order; ++n)
   {
      inverse.at(m_permut.at(n)) = n;
   }
   return Create(inverse);
}

Permutation::Entry Permutation::getOrder() const
{
   Permutation perm = *this;
   for (Entry result = 1; result < std::numeric_limits<Entry>::max(); ++result)
   {
      if (PermutationUtils::isIdentity(perm)) return result;
      perm = perm * (*this);
   }
   throw MyException("Permutation::getOrder error");
}

Permutation Permutation::getPower(int pow) const
{
   if (pow == 0) return Permutation::CreateTrivial(getCardinality());
   if (pow == 1) return *this;
   if (pow < 0) return getInverse().getPower(-pow);
   Permutation result = *this;
   for (int n = 1; n < pow; ++n)
   {
      result = result * *this;
   }
   return result;
}