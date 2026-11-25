#include "TextureIconManager.h"

#include <fstream>
#include <Renderer/RendererManager.h>

namespace Eclipse::Editor
{
	void PreviewManager::Init()
	{
		Graphics::IRenderer& rend = Graphics::RendererManager::GetRenderer();
		Graphics::IGraphicsDevice* device = rend.GetDevice();

		const std::filesystem::path path = PathManager::GetArtifactsPath() / "Previews";
		for (auto d : std::filesystem::recursive_directory_iterator(path))
		{
			Graphics::TextureDescription desc{};

			std::ifstream file(d.path(), std::ios::binary);
			file.read(reinterpret_cast<char*>(&desc.width), sizeof(int));
			file.read(reinterpret_cast<char*>(&desc.height), sizeof(int));
			file.read(reinterpret_cast<char*>(&desc.channels), sizeof(int));

			const int size = desc.width * desc.height * desc.channels;
			std::vector<Byte> bytes(size);
			file.read(reinterpret_cast<char*>(bytes.data()), size);

			Preview prev;
			prev.textureId = device->CreateTexture(bytes, desc);
			prev.height = desc.height;
			prev.width = desc.width;
		}
	}

	void PreviewManager::Update()
	{
		for (auto it = ActivePreviews.begin(); it != ActivePreviews.end(); it++ )
		{
			Preview& prev = *it;
			prev.lastFrameUsed++;

			if (prev.lastFrameUsed >= FramesToLive)
			{
				it = ActivePreviews.erase(it);
			}
		}
	}

	unsigned int PreviewManager::GetPreview(GUID aGuid)
	{
		if (const Preview* prev = TryGetPreviewData(aGuid))
			return prev->textureId;


		const Preview& preview = LoadPreview(aGuid);

		return preview.textureId;
	}

	const Preview& PreviewManager::GetPreviewData(GUID aGuid)
	{
		for (Preview& prev : ActivePreviews)
		{
			if (prev.guid == aGuid)
			{
				prev.lastFrameUsed = 0;
				return prev;
			}
		}

		return LoadPreview(aGuid);
	}

	const Preview* PreviewManager::TryGetPreviewData(GUID aGuid)
	{
		for (Preview& prev : ActivePreviews)
		{
			if (prev.guid == aGuid)
			{
				prev.lastFrameUsed = 0;
				return &prev;
			}
		}

		return nullptr;
	}

	const Preview& PreviewManager::LoadPreview(GUID aGuid)
	{
		Graphics::IRenderer& rend = Graphics::RendererManager::GetRenderer();
		Graphics::IGraphicsDevice* device = rend.GetDevice();

		const std::filesystem::path path = PathManager::GetArtifactsPath() / "Previews";
		Graphics::TextureDescription desc{};

		const std::string guid = aGuid.ToString();
		const std::filesystem::path previewPath = path / (guid + ".preview");

		std::ifstream file(path / (guid + ".preview"), std::ios::binary);
		file.read(reinterpret_cast<char*>(&desc.width), sizeof(int));
		file.read(reinterpret_cast<char*>(&desc.height), sizeof(int));
		file.read(reinterpret_cast<char*>(&desc.channels), sizeof(int));

		const int size = desc.width * desc.height * desc.channels;
		std::vector<Byte> bytes(size);
		file.read(reinterpret_cast<char*>(bytes.data()), size);

		Preview prev;
		prev.guid = aGuid;
		prev.textureId = device->CreateTexture(bytes, desc);
		prev.height = desc.height;
		prev.width = desc.width;

		ActivePreviews.push_back(prev);

		return ActivePreviews.back();
	}
}