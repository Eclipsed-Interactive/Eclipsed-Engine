#pragma once

namespace Eclipse::AssetSystem
{
	struct ArtifactHeader
	{
		U32 magic;
		U64 payloadSize;
	};
}