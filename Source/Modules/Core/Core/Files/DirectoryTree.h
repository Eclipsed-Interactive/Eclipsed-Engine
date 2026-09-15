#pragma once

#include <filesystem>
#include <vector>

#include "FileInfo.h"
#include "Core.Core.hpp"

namespace Eclipse::Utilities
{
	struct CORE_API FileNode
	{
		FileInfo info;
		std::vector<FileNode*> children;

		bool isDirectory = false;
		bool isOpen = false;
	};

	class CORE_API DirectoryTree final
	{
	public:
		DirectoryTree() = default;
		DirectoryTree(const std::filesystem::path& path);

		~DirectoryTree();

		DirectoryTree(const DirectoryTree&) = delete;
		DirectoryTree& operator=(const DirectoryTree&) = delete;

		DirectoryTree(DirectoryTree&& other) noexcept;
		DirectoryTree& operator=(DirectoryTree&& other) noexcept;

		void Reload();

	private:
		void Internal_Clear(FileNode* node);
		void Internal_SetupRoot(const std::filesystem::path& path);
		void Internal_BuildChildren(FileNode* node);

		const FileNode* Internal_GetNode(const std::filesystem::path& path, const FileNode* node);

	public:
		FileNode* GetRoot();
		const FileNode* GetRoot() const;

		const FileNode* GetNode(const std::filesystem::path& path);

		void Insert(const char* path);
		void Remove(const char* path);
		FileNode* Find(const char* path);

	private:
		FileNode* root = nullptr;
		std::filesystem::path relativePath;
	};
}