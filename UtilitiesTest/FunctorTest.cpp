#include <gtest/gtest.h>

#include "Utilities/Functors.h"

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

