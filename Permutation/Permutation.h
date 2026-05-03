#pragma once

#include <limits>
#include <span>
#include <vector>

class Permutation
{
  public:
    using Entry = unsigned int;
    using Cycle = std::vector<Entry>;
    using Cycles = std::vector<Cycle>;
    static constexpr Entry InvalidEntry = std::numeric_limits<Entry>::max();

    enum class Parity
    {
        EVEN,
        ODD
    };

    static Permutation CreateTrivial(Entry);
    static Permutation Create(std::initializer_list<Entry>);
    static Permutation Create(std::span<const Entry>);
    static Permutation CreateFromCycle(Entry, std::span<const Entry>);
    static Permutation CreateFromDisjunctCycles(Entry, std::initializer_list<std::initializer_list<Entry>> cyvles);
    static Permutation CreateRandomShuffle(const Permutation &, Entry seed);

    template <typename InputIt, typename OutputIt> void apply(InputIt first, OutputIt d_first) const;

    Entry operator()(Entry) const;
    Entry getCardinality() const;

    // perm^order = id
    Entry getOrder() const;
    Permutation getInverse() const;
    Permutation getPower(int pow) const;
    std::vector<std::vector<Entry>> getCycles() const;
    Parity getParity() const;

    static Permutation compose(const Permutation &perm1, const Permutation &perm0);
    friend Permutation operator*(const Permutation &, const Permutation &);
    friend bool operator==(const Permutation &, const Permutation &);
    friend std::strong_ordering operator<=>(const Permutation &, const Permutation &);

  private:
    explicit Permutation(std::vector<Entry> &&);
    std::vector<Entry> m_permut;
    Cycles m_cycles;
};

template <typename InputIt, typename OutputIt> void Permutation::apply(InputIt first, OutputIt d_first) const
{
    for (size_t n = 0; n < m_permut.size(); ++n)
    {
        d_first[n] = first[m_permut.at(n)];
    }
}
