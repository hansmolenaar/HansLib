#pragma once

#include "GraphDefines.h"
#include "IGraphUs.h"

namespace Graph
{

class UndirectedGraphColor
{
  public:
    using Color = unsigned int;
    static constexpr UndirectedGraphColor::Color ColorFirst = 0;

    explicit UndirectedGraphColor(const Graph::IGraphUs &);
    const Graph::IGraphUs &getGraph() const;
    std::optional<Color> getChromaticNumber() const;
    std::vector<Color> getColoring2() const;

  private:
    const Graph::IGraphUs &m_graph;
    std::optional<Color> m_chromaticNumber;
    std::vector<Color> m_colors;
};

} // namespace Graph
