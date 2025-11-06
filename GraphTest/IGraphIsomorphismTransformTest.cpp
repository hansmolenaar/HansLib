#include <gtest/gtest.h>

#include "IGraphIsomorphismTransform.h"
#include "Single.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

TEST(IGraphIsomorphismTransformTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto transformed = IGraphIsomorphismTransform::Create(*graph);
    const auto &tag = Single(transformed->getChildTags());
    const auto &children = transformed->getChildren(tag);
    ASSERT_EQ(children.size(), 2);
    auto tmp0 = children.at(0)->getVertexInParent(0);
    auto tmp1 = children.at(1)->getVertexInParent(1);
}
