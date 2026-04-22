#ifdef ECLIPSED_EDITOR
#include "TextureIconManager.h"

#include <filesystem>
#include <fstream>

#include <OpenGL/glad/glad.h>

#include "CoreEngine/Files/FileWatcher.h"

#include "CoreEngine/PathManager.h"
#include "CoreEngine/Files/FileInfo.h"
#include "CoreEngine/Files/File.h"
//#include "CoreEngine/AssetManagement/Resources.h"

#include "AssetEngine/Helper/STB_Helper.h"

namespace Eclipse::Editor
{
	void IconManager::CheckForChanges()
	{

	}

	void IconManager::Update()
	{
		namespace fs = std::filesystem;

		for (auto it = filesToLoad.begin(); it != filesToLoad.end();)
		{
			const std::string path(it->c_str());
			if (IsFileReady(path))
			{
				std::string relativePath = fs::relative(path, PathManager::GetProjectRoot()).generic_string();

				size_t id = std::hash<std::string>{}(relativePath);

				if (loadedIcons.find(id) == loadedIcons.end())
				{
					if (!LoadTextureIconFromPath(path))
					{
						it++;
						continue;
					}
					ExportLoadedTextures();

					it = filesToLoad.erase(it);
				}
				else
				{
					it++;
				}
			}
			else
			{
				it++;
			}
		}

	}

	void IconManager::LoadIconsFile()
	{
		namespace fs = std::filesystem;

		std::ifstream in(PathManager::GetProjectLocalPath() / IconBundleFilePath, std::ios::binary);

		int count = 0;
		in.read(reinterpret_cast<char*>(&count), sizeof(int));

		if (count == 0)
		{
			in.close();

			LoadAllTextureIcons();
			ExportLoadedTextures();
			return;
		}

		for (int i = 0; i < count; ++i)
		{
			size_t id;
			in.read(reinterpret_cast<char*>(&id), sizeof(size_t));

			IconData& data = loadedIcons[id];

			in.read(reinterpret_cast<char*>(&data.channels), sizeof(unsigned));
			in.read(reinterpret_cast<char*>(&data.width), sizeof(int));
			in.read(reinterpret_cast<char*>(&data.height), sizeof(int));
			in.read(reinterpret_cast<char*>(&data.lastWriteTime), sizeof(long long));

			const int size = data.width * data.height * data.channels;
			data.data.resize(size);
			in.read(reinterpret_cast<char*>(data.data.data()), size);

			CreateOpenGLTextureFromData(data);
		}

		in.close();
	}

	void IconManager::LoadAllTextureIcons()
	{
		namespace fs = std::filesystem;

		std::vector<std::string> texturesPathsToLoad;
		for (const fs::directory_entry& entry : fs::recursive_directory_iterator(PathManager::GetAssetsPath()))
		{
			Utilities::FileInfo info = Utilities::FileInfo::GetFileInfo(entry);
			if (info.type != Utilities::FileInfo::FileType_Texture) continue;

			texturesPathsToLoad.push_back(entry.path().string());
		}

		for (const std::string& path : texturesPathsToLoad)
		{
			LoadTextureIconFromPath(path);
		}
	}

	bool IconManager::LoadTextureIconFromPath(const std::filesystem::path& path)
	{
		namespace fs = std::filesystem;

		std::string relativePath = fs::relative(path, PathManager::GetAssetsPath()).generic_string();

		size_t id = std::hash<std::string>{}(relativePath);

		IconData& data = loadedIcons[id];

		{ // Get last write time.
			fs::file_time_type ftime = fs::last_write_time(path);

			std::chrono::system_clock::time_point sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
				ftime - fs::file_time_type::clock::now()
				+ std::chrono::system_clock::now()
			);

			data.lastWriteTime = std::chrono::system_clock::to_time_t(sctp);
		}

		{ // Load texture
			int width = 0, height = 0;
			unsigned char* pixels = STB_Helper::Load_Texture_STB(path.string().c_str(), width, height, data.channels, false);
			if (!pixels)
			{
				loadedIcons.erase(id);
				return false;
			}

			if (width >= height)
			{
				data.width = 128.f;
				data.height = (float)height / (float)width * 128.f;
			}
			else
			{
				data.height = 128.f;
				data.width = (float)width / (float)height * 128.f;
			}

			unsigned char* resized = new unsigned char[width * height * data.channels];
			resized = STB_Helper::Resize_STB(pixels, width, height, data.width, data.height, data.channels);


			const int size = data.width * data.height * data.channels;
			data.data.resize(size);
			memcpy(data.data.data(), resized, size);

			STB_Helper::FreeData_STB(resized);
		}

		CreateOpenGLTextureFromData(data);
		return true;
	}

	void IconManager::ExportLoadedTextures()
	{
		namespace fs = std::filesystem;

		auto path = PathManager::GetProjectLocalPath() / IconBundleFilePath;
		std::string pathString = path.generic_string();

		{
			Utilities::File file(pathString.c_str(), std::ios::binary | std::ios::out);
		}

		std::ofstream out(path, std::ios::binary);

		const int count = (int)loadedIcons.size();
		out.write(reinterpret_cast<const char*>(&count), sizeof(int));

		for (auto& [id, data] : loadedIcons)
		{
			out.write(reinterpret_cast<const char*>(&id), sizeof(size_t));
			out.write(reinterpret_cast<const char*>(&data.channels), sizeof(unsigned));
			out.write(reinterpret_cast<const char*>(&data.width), sizeof(unsigned));
			out.write(reinterpret_cast<const char*>(&data.height), sizeof(unsigned));
			out.write(reinterpret_cast<const char*>(&data.lastWriteTime), sizeof(long long));

			out.write(reinterpret_cast<const char*>(data.data.data()), data.width * data.height * data.channels);
		}

		out.close();
	}

	bool IconManager::IsFileReady(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::binary);
		return file.is_open() && file.peek() != std::ifstream::traits_type::eof();
	}

	void IconManager::CreateOpenGLTextureFromData(IconData& data)
	{
		glGenTextures(1, &data.textureID);
		glBindTexture(GL_TEXTURE_2D, data.textureID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLenum format;
		if (data.channels == 1)
			format = GL_RED;
		else if (data.channels == 2)
			format = GL_RG;
		else if (data.channels == 3)
			format = GL_RGB;
		else if (data.channels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, data.width,
			data.height, 0, format, GL_UNSIGNED_BYTE, data.data.data());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const IconData& IconManager::GetIcon(const Utilities::FileInfo& fileInfo)
	{
		size_t id = std::hash<std::string>{}(fileInfo.relativeFilePath.generic_string());

		if (loadedIcons.find(id) == loadedIcons.end()) return emptyInternalIconData;


		return loadedIcons[id];
	}
}
#endif