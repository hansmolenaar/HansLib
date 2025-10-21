#include "Combinations.h"

#include <algorithm>

std::vector<std::vector<bool>> Combinations::Get(int N, int K)
{
    std::vector<std::vector<bool>> result;
    std::vector<bool> bitmask(K, true); // K leading 1's
    bitmask.resize(N, false);           // N-K trailing 0's

    // permute bitmask
    do
    {
        result.emplace_back(bitmask);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    return result;
}
