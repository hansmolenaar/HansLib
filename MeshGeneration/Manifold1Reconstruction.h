#pragma once

#include "TrianglesNodes.h"
#include "MeshGenerationDefines.h"

namespace Manifold1Reconstruction
{
	struct Reconstruction
	{
		std::vector<MeshGeneration::NodeIndex> Singletons; // TODO remove me?
		std::vector<std::vector<MeshGeneration::NodeIndex>> Cycles;
		std::vector<std::vector<MeshGeneration::NodeIndex>> Paths;
	};

	Reconstruction Generate2(std::span<const MeshGeneration::NodeIndex> manifoldPoints, const MeshGeneration::TrianglesNodes& trianglesNodes, const MeshGeneration::IUniquePointCollecion2& pointCollection);
}