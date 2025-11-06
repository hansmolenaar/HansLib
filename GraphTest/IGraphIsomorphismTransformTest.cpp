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

    ASSERT_EQ(children.at(0)->getSelf().getNumVertices(), 1);
    ASSERT_EQ(children.at(0)->getVertexInParent(0), 0);

    ASSERT_EQ(children.at(1)->getSelf().getNumVertices(), 1);
    ASSERT_EQ(children.at(1)->getVertexInParent(0), 1);
}
