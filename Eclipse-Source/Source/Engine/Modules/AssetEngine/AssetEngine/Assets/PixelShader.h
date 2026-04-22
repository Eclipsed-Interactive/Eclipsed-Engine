#pragma once

#include "AssetEngine/Data/ShaderData.h"
#include "BaseAsset.h"

namespace Eclipse
{
	class PixelShader
	{
		BASE_ASSET(PixelShader);

	public:
		unsigned GetProgramID();

		void Bind();
	};
}