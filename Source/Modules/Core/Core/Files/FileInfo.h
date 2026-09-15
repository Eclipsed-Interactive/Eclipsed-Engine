#pragma once

#include <filesystem>
#include "Core.Core.hpp"

namespace Eclipse::Utilities
{
	struct CORE_API FileInfo
	{
		enum FileType_
		{
			FileType_None,
			FileType_Directory,

			FileType_Zip,
			FileType_Json,
			FileType_Scene,
			FileType_Texture,
			FileType_Audio,
			FileType_Material,
			FileType_Prefab,
			FileType_Font,
			FileType_VertexShader,
			FileType_PixelShader,
			FileType_SpriteAnimation,
			FileType_CSource,
			FileType_HSource,
			FileType_INLSource,
		};

		enum FileStatus_
		{
			FileStatus_NotFound,
			FileStatus_Ok,
			FileStatus_Directory,
			FileStatus_Symlink,
			FileStatus_Special,
			FileStatus_Unknown,
			FileStatus_NoPerm,
		};

		FileStatus_ status;
		FileType_ type;
		std::filesystem::path filePath;
		std::filesystem::path relativeFilePath;
		
		std::string assetID;

	public:
		const char* GetIcon() const;

		static FileInfo GetFileInfo(const std::filesystem::path& aPath);
		void SetRelativePath(const std::filesystem::path& rel);

	};
}