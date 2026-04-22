#include "ShaderLoader.h"

#include "AssetEngine/Helper/ShaderCompiler.h"

namespace Eclipse
{
	template<typename T, AssetType Type>
	T* ShaderLoader<T, Type>::Load(std::ifstream& in) const
	{
		if (!in.is_open()) return nullptr;

		static_assert(
			Type == AssetType::VertexShader ||
			Type == AssetType::PixelShader,
			"ShaderLoader only supports VertexShader and PixelShader"
			);

		int type = 0;
		if constexpr (Type == AssetType::VertexShader) type = GL_VERTEX_SHADER;
		else if constexpr (Type == AssetType::PixelShader) type = GL_FRAGMENT_SHADER;

		T* data = new T;

		size_t size = 0;
		std::string shaderSource = "";

		in.read(reinterpret_cast<char*>(&size), sizeof(size_t));

		shaderSource.resize(size);
		in.read(shaderSource.data(), size);

		ShaderCompileInfo info = ShaderCompiler::CompileShaderFromMemory(type, shaderSource.c_str());
		data->shaderProgramID = info.shaderID;




		//int type = (int)Type;
		//outStream.write(reinterpret_cast<const char*>(&type), sizeof(int));

		//size_t shadeSize = shaderSource.size();
		//outStream.write(reinterpret_cast<const char*>(&shadeSize), sizeof(size_t));
		//outStream.write(reinterpret_cast<const char*>(shaderSource.c_str()), shaderSource.size());



		return data;
	}
}