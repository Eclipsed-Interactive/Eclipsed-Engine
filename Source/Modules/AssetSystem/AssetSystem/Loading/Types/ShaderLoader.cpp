#include "ShaderLoader.h"

#include <AssetSystem/ShaderType.h>
#include <Core/Standard/Core/Types.hpp>
#include "ArtifactHeader.h"

#include <Renderer/RendererManager.h>

#include <Renderer/RendererManager.h>

namespace Eclipse::AssetSystem
{
	bool ShaderLoader::Load(ArtifactReader& aReader, AssetData* aData) const
	{
		ShaderData* shader = static_cast<ShaderData*>(aData);

		if (!aReader.Read(&shader->type, sizeof(ShaderType))) return false;

		Graphics::ShaderStage stage;
		switch (shader->type)
		{
		case ShaderType::VertexShader:   stage = Graphics::Vertex;   break;
		case ShaderType::PixelShader:    stage = Graphics::Pixel; break;
		case ShaderType::GeometryShader: stage = Graphics::Geometry; break;
		case ShaderType::ComputeShader:  stage = Graphics::Compute;  break;
		}

		Size wordCount = 0;
		if (!aReader.Read(&wordCount, sizeof(Size))) return false;

		const Size byteSize = wordCount * sizeof(uint32_t);
		if (aReader.Offset() + byteSize > aReader.Capacity()) return false;

		std::span<const Byte> spirv{ reinterpret_cast<const Byte*>(aReader.Current()), byteSize };
		aReader.Skip(byteSize);
		
		Graphics::IRenderer& renderer = Graphics::RendererManager::GetRenderer();
		Graphics::IGraphicsDevice* device = renderer.GetDevice();
		shader->shaderID = device->CreateShader(spirv, stage);

		return true;
	}
	bool ShaderLoader::Unload(AssetData* aData) const
	{
		return false;
	}
}