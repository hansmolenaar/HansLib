#include <gtest/gtest.h>

#include "Single.h"
#include "UniquePointer.h"
#include <vector>

using namespace Utilities;

namespace
{
class foo
{
  public:
    virtual ~foo() = default;
};

class bar : public foo
{
};

} // namespace

TEST(UniquePointerdTest, Test)
{
    std::vector<std::unique_ptr<int>> values;

    std::vector<int *> pointers = getPointers(values);
    ASSERT_TRUE(pointers.empty());
    std::vector<const int *> cpointers = getCPointers(values);
    ASSERT_TRUE(cpointers.empty());

    values.emplace_back(new int);
    *values.front() = 42;

    pointers = getPointers(values);
    ASSERT_EQ(*Single(pointers), 42);
    cpointers = getCPointers(values);
    ASSERT_EQ(*Single(cpointers), 42);
}

TEST(UniquePointerdTest, Cast)
{
    std::vector<std::unique_ptr<foo>> values;
    std::vector<const bar *> pointers = getCastPointers<const bar>(values);
    ASSERT_TRUE(pointers.empty());
}
