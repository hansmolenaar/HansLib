#include "IntervalTreeIndex1.h"

#include "MyAssert.h"
#include "MyException.h"
#include <cmath>

using namespace IntervalTree;

namespace
{
Level GetLevel(const Interval<Rational> &interval)
{
    if (interval.getLower() < 0)
    {
        throw MyException("IntervalTreeIndex::GetLevel negative lowr bound");
    }

    const auto dif = interval.getUpper() - interval.getLower();
    if (dif.numerator() != 1)
    {
        throw MyException("IntervalTreeIndex::GetLevel interval not unit size");
    }

    const Level result = static_cast<Level>(std::round(std::log2(dif.denominator())));
    if (1 << result != dif.denominator())
    {
        throw MyException("GetLevel: incorrect interval");
    }
    return result;
}

int GetPositionInLevel(const Interval<Rational> &interval, Level level)
{
    if (interval.getLower().denominator() >= interval.getUpper().denominator())
    {
        return interval.getLower().numerator();
    }
    return interval.getUpper().numerator() - 1;
}
} // namespace

Index1::Index1(const Interval<Rational> &interval)
    : m_interval(interval), m_level(GetLevel(m_interval)), m_positionInLevel(GetPositionInLevel(m_interval, m_level)),
      m_key(composeKey(m_level, m_positionInLevel))
{
    // Final checks
    const auto denom = 1 << getLevel();
    if (m_interval.getLower() != Rational(getPositionInLevel(), denom))
    {
        throw MyException("Index1::Index1 incorrect interval, lower");
    }
    if (m_interval.getUpper() != Rational(getPositionInLevel() + 1, denom))
    {
        throw MyException("Index1::Index1 incorrect interval, upper");
    }
}

Index1 Index1::Create(const Interval<Rational> &interval)
{
    return {interval};
}

Index1 Index1::CreateFromKey(Key key)
{
    const auto [level, pos] = Index1::decomposeKey(key);
    const auto pow2 = 1 << level;
    return Create(Interval<Rational>({pos, pow2}, {pos + 1, pow2}));
}

Level Index1::GetLevelFromKey(Key key)
{
    const auto [level, pos] = Index1::decomposeKey(key);
    return level;
}

Level Index1::getLevel() const
{
    return m_level;
}

int Index1::getPositionInLevel() const
{
    return m_positionInLevel;
}

std::tuple<Level, int> Index1::decomposeKey(Key key)
{
    int maxKey = 0;
    for (Level level = 0; level < 256; ++level)
    {
        if (key < maxKey + (1 << level))
            return std::make_tuple(level, key - maxKey);
        maxKey += 1 << level;
    }
    throw MyException("Index1::decomposeKey should not come here");
}

Index1::Key Index1::composeKey(Level level, int positionInLevel)
{
    const Index1::Key pow2m1 = (1 << level) - 1;
    return pow2m1 + positionInLevel;
}

Index1::Key Index1::getKey() const
{
    return m_key;
}

const Interval<Rational> &Index1::getInterval() const
{
    return m_interval;
}

std::array<Index1::Key, 2> Index1::refine(Index1::Key key)
{
    const auto [level, pos] = decomposeKey(key);
    return {composeKey(level + 1, 2 * pos), composeKey(level + 1, 2 * pos + 1)};
}

std::array<Index1, 2> Index1::refine() const
{
    const auto keys = refine(getKey());
    return {CreateFromKey(keys[0]), CreateFromKey(keys[1])};
}

Rational Index1::getMeasure() const
{
    return m_interval.getMeasure();
}

bool Index1::IsRoot(Key key)
{
    return key == Index1::KeyRoot;
}

bool Index1::isRoot() const
{
    return IsRoot(m_key);
}

Index1 Index1::CreateRoot()
{
    return Index1(Interval<Rational>{{0, 1}, {1, 1}});
}

Rational Index1::getCenter() const
{
    return m_interval.interpolate(Rational(1, 2));
}

Index1 Index1::getSibling() const
{
    Utilities::MyAssert(!isRoot());
    if (m_positionInLevel % 2 == 0)
    {
        auto result = getAdjacentInDir(true);
        return *result;
    }
    else
    {
        auto result = getAdjacentInDir(false);
        return *result;
    }
}

std::optional<Index1> Index1::getSiblingInDir(bool posDir) const
{
    const bool posKid = (m_positionInLevel % 2 != 0);
    if (m_level == 0 || posKid == posDir)
        return {};
    return getSibling();
}

std::optional<Index1> Index1::getAdjacentInDir(bool posDir) const
{
    if (m_level == 0)
        return {};
    if (posDir)
    {
        if (m_positionInLevel + 1 >= (1 << m_level))
            return {};
        const Rational upr = m_interval.getUpper();
        return Index1(Interval<Rational>(upr, upr + getMeasure()));
    }
    else
    {
        if (m_positionInLevel == 0)
            return {};
        const Rational lwr = m_interval.getLower();
        return Index1(Interval<Rational>(lwr - getMeasure(), lwr));
    }
}

Index1::Key Index1::GetParentKey(Index1::Key key)
{
    Utilities::MyAssert(key != KeyRoot);
    const auto [level, pos] = Index1::decomposeKey(key);
    return composeKey(level - 1, pos / 2);
}
Index1 Index1::getParent() const
{
    return CreateFromKey(getParentKey());
}

Index1::Key Index1::getParentKey() const
{
    return GetParentKey(getKey());
}