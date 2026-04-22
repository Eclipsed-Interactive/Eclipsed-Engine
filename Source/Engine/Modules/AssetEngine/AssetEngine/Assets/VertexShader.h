#pragma once

#include "AssetEngine/Data/ShaderData.h"
#include "BaseAsset.h"


namespace Eclipse
{
	class VertexShader
	{
		BASE_ASSET(VertexShader);

	public:
		unsigned GetProgramID() const;

		void Bind();
	};
}