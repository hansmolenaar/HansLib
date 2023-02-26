#include <gtest/gtest.h>

#include "Functors.h"
#include <limits>

TEST(FunctorTest, IsEqualTo)
{
   const Functors::IsEqualTo isEqualTo42{ 42 };
   ASSERT_FALSE(isEqualTo42(1));
   ASSERT_TRUE(isEqualTo42(42));
}

struct Base 
{
   virtual ~Base() = default;
};
struct Derived_1 : public Base {};
struct Derived_2 : public Base {};


struct AnotherBase
{
   virtual ~AnotherBase() = default;
};

TEST(FunctorTest, PointerIsOfType)
{
   //const Functors::PointerIsOfType<int> isIntPointer;

   const Derived_1 d1;
   const Derived_2 d2;
   const Base* b1 = &d1;
   const Base* b2 = &d2;

   const AnotherBase ab;
  ASSERT_TRUE( Functors::IsOfDerivedType<Derived_1>()(b1));
  ASSERT_FALSE(Functors::IsOfDerivedType<Derived_1>()(b2));
  ASSERT_FALSE(Functors::IsOfDerivedType<Derived_1>()(&ab));
}

TEST(FunctorTest, AreClose)
{
   constexpr Functors::AreClose areClose;
   ASSERT_TRUE(areClose(0,0));
   ASSERT_TRUE(areClose(0, std::numeric_limits<double>::min()));
   ASSERT_FALSE(areClose(0, 1.0));
   ASSERT_FALSE(areClose(-1.0, 1.0));
}


