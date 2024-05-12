#pragma once

#include "TrianglesNodes.h"
#include "MeshGeneration2Defines.h"

namespace Manifold1Reconstruction
{
	struct Reconstruction
	{
		std::vector<MeshGeneration::NodeIndex> Singletons;
		std::vector<std::vector<MeshGeneration::NodeIndex>> Cycles;
		std::vector<std::vector<MeshGeneration::NodeIndex>> Paths;
	};

	Reconstruction Generate2(std::span<const MeshGeneration::NodeIndex> manifoldPoints, const MeshGeneration::TrianglesNodes& trianglesNodes, MeshGeneration2::IUniquePointCollecion2& pointCollection);
}