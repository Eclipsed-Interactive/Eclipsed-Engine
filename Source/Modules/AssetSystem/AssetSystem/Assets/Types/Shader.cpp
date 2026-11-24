#include "Shader.h"

#include <AssetSystem/Assets/Data/ShaderData.hpp>

namespace Eclipse
{
	Shader::Shader(AssetSystem::AssetData* aData)
		: myGuid(aData->guid), myData(reinterpret_cast<AssetSystem::ShaderData*>(aData))
	{
	}
	
	unsigned int Shader::ShaderID() const
	{
		return myData->shaderID;
	}
	
	AssetSystem::ShaderType Shader::Type() const
	{
		return myData->type;
	}
	
	bool Shader::IsReady() const
	{
		return myData != nullptr;
	}
}
