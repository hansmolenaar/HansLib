#include <gtest/gtest.h>

#include "GroupFromOrbit.h"
#include "PointTransformationIdentity.h"
#include "PointClose.h"
#include "Rotate2D.h"
#include "CyclicGroup.h"
#include "SameFiniteGroupPredicate.h"
#include "ActionDihedral.h"
#include "DihedralGroup.h"
#include "PointTransformationFromGroupAction.h"

#include <numbers>

TEST(GroupFromOrbitTest, Trivial)
{
   const  PointTransformationIdentity<double, 1> transform;
   const  std::vector<const IPointTransformation<double, 1>*> transformations{ &transform };
   const PointClose<double, 1> areClose;
   const auto retval = GroupFromOrbit::Create(areClose, transformations, Point1{ 3.0 });

   ASSERT_EQ(retval->getOrder(), 1);
   ASSERT_EQ(retval->getIdentity(), 0);
}

TEST(GroupFromOrbitTest, Rotate4)
{
   const  Rotate2D rotate0(0);
   const  Rotate2D rotate1(std::numbers::pi/2);
   const  Rotate2D rotate2(std::numbers::pi );
   const  Rotate2D rotate3(3*std::numbers::pi/2);
   const  std::vector<const IPointTransformation<double, 2>*> transformations{ &rotate0, &rotate1, &rotate2, &rotate3 };
   const PointClose<double, 2> areClose;
   const auto retval = GroupFromOrbit::Create(areClose, transformations, Point2{ 1, 0 });

   ASSERT_EQ(retval->getOrder(), 4);
   ASSERT_EQ(retval->getIdentity(), 0);
   const auto expect = CyclicGroup::Create(4);
   const SameFiniteGroupPredicate areSame{*expect};
   ASSERT_TRUE(areSame(*retval));
}

TEST(GroupFromOrbitTest, Di3)
{
   const ActionDihedral action(3);
   const PointClose<double, 2> areClose;
   std::vector<PointTransformationFromGroupAction<2>> transformationsFromAction; 
   for (int n = 0; n < action.getGroup().getOrder(); ++n)
   {
      transformationsFromAction.emplace_back(PointTransformationFromGroupAction(action, n));
   }

   std::vector<const IPointTransformation<double, 2>* > transformations;
   for (int n = 0; n < action.getGroup().getOrder(); ++n)
   {
      transformations.push_back(transformationsFromAction.data() + n);
   }

   const auto retval = GroupFromOrbit::Create(areClose, transformations, Point2{ 1, 2 });
   ASSERT_TRUE(retval);

   const SameFiniteGroupPredicate areEqual(*retval);
   ASSERT_TRUE(areEqual(*DihedralGroup::Create(3)));
}

