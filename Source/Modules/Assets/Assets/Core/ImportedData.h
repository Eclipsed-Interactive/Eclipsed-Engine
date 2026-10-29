#pragma once

#include <vector>
#include <variant>

#include "Assets/Core/GUID.h"
#include "Core/Math/Color.h"

#include "Core/Math/RectSizePos.h"

namespace Eclipse::Assets
{
#pragma region AUDIO

	enum class AudioStorage
	{
		PCM,
		Compressed
	};

	enum class AudioFormat
	{
		PCM16,
		PCM32F,
		OGG,
		WAV,
		MP3
	};

	struct ImportedAudio
	{
		AudioStorage Storage = AudioStorage::PCM;
		AudioFormat Format = AudioFormat::PCM16;

		unsigned int SampleRate = 0;
		unsigned int FrameCount = 0;
		int Channels = 0;

		bool Streaming = false;

		std::vector<unsigned char> Data;
	};

#pragma endregion

#pragma region FONT

	struct ImportedFont
	{
		std::vector<unsigned char> Data;
	};

#pragma endregion

#pragma region MATERIAL

	struct ImportedMaterial {
		GUID textureID;
		GUID vertexShaderID;
		GUID pixelShaderID;
		Math::Color color;
	};

#pragma endregion

#pragma region TEXTURE

	struct ImportedTexture {
		int width;
		int height;
		int channels;
		std::vector<unsigned char> Data;

		std::vector<Math::RectSizePos> spriteRects;
	};

#pragma endregion

#pragma region PREFAB

	struct ImportedPrefab
	{
		std::vector<unsigned char> Data;
	};

#pragma endregion

#pragma region SHADER

	struct ImportedShader
	{
		std::string source;
	};

#pragma endregion

#pragma region SCENE

	struct ImportedScene
	{
		std::string source;
		std::string name;
	};

#pragma endregion



	using ImportedData = std::variant<
		ImportedAudio,
		ImportedFont,
		ImportedMaterial,
		ImportedTexture,
		ImportedPrefab,
		ImportedShader,
		ImportedScene
	>;

	using ProcessedData = ImportedData;
}