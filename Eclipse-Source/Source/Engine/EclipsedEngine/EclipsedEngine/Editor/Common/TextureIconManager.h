
#ifdef ECLIPSED_EDITOR
#pragma once

#include <filesystem>

namespace Eclipse::Editor
{
	struct IconData
	{
		bool exists = false;
		unsigned textureID = 0;
		int channels;
		int width;
		int height;
		long long lastWriteTime;
		std::vector<unsigned char> data;
	};

	class IconManager final
	{
	public:
		static void CheckForChanges();
		static void Update();
		static void LoadIconsFile();
		static void LoadAllTextureIcons();
		static bool LoadTextureIconFromPath(const std::filesystem::path& path);
		static void ExportLoadedTextures();

		static bool IsFileReady(const std::filesystem::path& path);

		static void CreateOpenGLTextureFromData(IconData& data);

		static const IconData& GetIcon(const Utilities::FileInfo& fileInfo);

	private:
		static inline std::vector<std::string> filesToLoad;
		static inline char buffer[1024];
		static inline unsigned long bytesReturned;

		static inline const char* IconBundleFilePath = "Icons.bundle";
		static inline std::unordered_map<size_t, IconData> loadedIcons;

	private:
		static inline IconData emptyInternalIconData = {};
	};
}
#endif