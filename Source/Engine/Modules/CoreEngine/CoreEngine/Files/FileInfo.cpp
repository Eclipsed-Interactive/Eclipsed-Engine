#include "pch.h"

#include "FileInfo.h"

namespace Eclipse::Utilities 
{
	const char* FileInfo::GetIcon() const
	{
		static std::unordered_map<FileInfo::FileType_, const char*> fileTypeToIcon =
		{
			{FileInfo::FileType_Audio, ICON_FA_FILE_AUDIO},
			{FileInfo::FileType_PixelShader, ICON_FA_QUESTION},
			{FileInfo::FileType_VertexShader, ICON_FA_QUESTION},
			{FileInfo::FileType_Texture, ICON_FA_IMAGE},
			{FileInfo::FileType_Scene, ICON_FA_EARTH_EUROPE},
			{FileInfo::FileType_Font, ICON_FA_FONT},
			{FileInfo::FileType_SpriteAnimation, ICON_FA_PERSON_RUNNING},
			{FileInfo::FileType_Directory, ICON_FA_FOLDER},
			{FileInfo::FileType_Json, ICON_FA_FILE_EXCEL },
			{FileInfo::FileType_Zip, ICON_FA_FILE_ZIPPER},

			{FileInfo::FileType_CSource, ICON_FA_FILE_CODE},
			{FileInfo::FileType_HSource, ICON_FA_FILE_LINES},
			{FileInfo::FileType_INLSource, ICON_FA_FILE_PEN},
		};

		const auto& it = fileTypeToIcon.find(type);
		if (it != fileTypeToIcon.end())
		{
			return it->second;
		}

		return ICON_FA_TRIANGLE_EXCLAMATION;
	}


	FileInfo FileInfo::GetFileInfo(const std::filesystem::path& aPath)
	{
		static std::unordered_map<std::string, FileInfo::FileType_> files =
		{
			{".png", FileInfo::FileType_Texture},
			{".jpg", FileInfo::FileType_Texture},
			{".jpeg", FileInfo::FileType_Texture},
			{".bmp", FileInfo::FileType_Texture},
			{".tga", FileInfo::FileType_Texture},
			{".gif", FileInfo::FileType_Texture},
			{".psd", FileInfo::FileType_Texture},
			{".hdr", FileInfo::FileType_Texture},

			{".vglsl", FileInfo::FileType_VertexShader},
			{".pglsl", FileInfo::FileType_PixelShader},

			{".h", FileInfo::FileType_HSource},
			{".hpp", FileInfo::FileType_HSource},

			{".c", FileInfo::FileType_CSource},
			{".cpp", FileInfo::FileType_CSource},

			{".inl", FileInfo::FileType_INLSource},

			{".scene", FileInfo::FileType_Scene},
			{".eprf", FileInfo::FileType_Prefab},

			{".wav", FileInfo::FileType_Audio},
			{".mp3", FileInfo::FileType_Audio},

			{".mat", FileInfo::FileType_Material},

			{".zip", FileInfo::FileType_Zip},

			{".json", FileInfo::FileType_Json},

			{".ttf", FileInfo::FileType_Font},
			{".TTF", FileInfo::FileType_Font},

			{".spriteanim", FileInfo::FileType_SpriteAnimation},

			{".spriteanim", FileInfo::FileType_SpriteAnimation},
		};

		FileInfo info;
		info.filePath = aPath;

		std::filesystem::file_status status = std::filesystem::status(aPath);
		switch (status.type())
		{
		case std::filesystem::file_type::not_found: info.status = FileInfo::FileStatus_NotFound; break;
		case std::filesystem::file_type::regular:
		{
			info.status = FileInfo::FileStatus_Ok;
			info.type = files[aPath.filename().extension().string()];
		}break;
		case std::filesystem::file_type::directory:
		{
			info.status = FileInfo::FileStatus_Directory;
			info.type = FileInfo::FileType_Directory;
		}break;
		case std::filesystem::file_type::symlink:   info.status = FileInfo::FileStatus_Symlink; break;

		case std::filesystem::file_type::block:
		case std::filesystem::file_type::character:
		case std::filesystem::file_type::fifo:
		case std::filesystem::file_type::socket:
			info.status = FileInfo::FileStatus_Special; break;

		default: info.status = FileInfo::FileStatus_Unknown; break;
		}

		return info;
	}
	void FileInfo::SetRelativePath(const std::filesystem::path& rel)
	{
		relativeFilePath = std::filesystem::relative(filePath, rel);
	}
}