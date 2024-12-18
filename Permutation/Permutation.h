#pragma once

#include <vector>
#include <span>

class Permutation
{
public:
   using Entry = unsigned char;
   static constexpr Entry InvalidEntry = std::numeric_limits<Entry>::max();

   static Permutation CreateTrivial(Entry);
   static Permutation Create(std::span<const Entry>);
   static Permutation CreateFromCycle(Entry, std::span<const Entry>);
   static Permutation CreateFromDisjunctCycles(Entry, std::initializer_list<std::initializer_list<Entry>> cyvles);

   template<typename InputIt, typename OutputIt>
   void apply(InputIt first, OutputIt d_first) const;

   Entry operator()(Entry) const;
   Entry getCardinality() const;

   friend Permutation operator*(const Permutation&, const Permutation&);
   friend bool operator==(const Permutation&, const Permutation&);
   friend  std::strong_ordering operator<=>(const Permutation&, const Permutation&);
private:
   explicit Permutation(std::vector<Entry>&&);
   std::vector<Entry> m_permut;
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