#include "Core/Files/DirectoryTree.h"

namespace Eclipse::Utilities
{
	namespace fs = std::filesystem;

	DirectoryTree::DirectoryTree(const std::filesystem::path& path)
		: relativePath(path)
	{
		Internal_SetupRoot(path);
		Internal_BuildChildren(root);

		if (root == nullptr)
		{
			return;
		}
	}

	DirectoryTree::~DirectoryTree()
	{
		Internal_Clear(root);
	}

	DirectoryTree::DirectoryTree(DirectoryTree&& other) noexcept
	{
		root = other.root;
		relativePath = std::move(other.relativePath);

		other.root = nullptr;
	}

	DirectoryTree& DirectoryTree::operator=(DirectoryTree&& other) noexcept
	{
		if (this != &other)
		{
			Internal_Clear(root);

			root = other.root;
			relativePath = std::move(other.relativePath);

			other.root = nullptr;
		}

		return *this;
	}

	void DirectoryTree::Reload()
	{
		Internal_Clear(root);

		Internal_SetupRoot(relativePath);
		Internal_BuildChildren(root);
	}

	void DirectoryTree::Internal_Clear(FileNode* node)
	{
		if (!node)
			return;

		for (FileNode* child : node->children)
		{
			Internal_Clear(child);
		}

		delete node;
	}

	void DirectoryTree::Insert(const char* path)
	{
	}

	void DirectoryTree::Remove(const char* path)
	{
	}

	FileNode* DirectoryTree::Find(const char* path)
	{
		return nullptr;
	}

	void DirectoryTree::Internal_SetupRoot(const std::filesystem::path& path)
	{
		root = new FileNode();

		root->info = FileInfo::GetFileInfo(path);
		root->isDirectory = root->info.type == FileInfo::FileType_Directory;
	}

	void DirectoryTree::Internal_BuildChildren(FileNode* node)
	{
		if (!node)
			return;

		for (const fs::directory_entry& entry : fs::directory_iterator(node->info.filePath))
		{
			if (entry.path().extension() == ".meta")
				continue;

			FileNode* child = new FileNode();

			child->info = FileInfo::GetFileInfo(entry);
			child->info.SetRelativePath(relativePath);

			child->isDirectory = entry.is_directory();

			if (child->isDirectory)
			{
				Internal_BuildChildren(child);
			}

			node->children.push_back(child);
		}

		std::sort(
			node->children.begin(),
			node->children.end(),
			[](const FileNode* a, const FileNode* b)
			{
				if (a->isDirectory != b->isDirectory)
					return a->isDirectory > b->isDirectory;

				if (a->info.type != b->info.type)
					return a->info.type > b->info.type;

				return a->info.filePath.filename() < b->info.filePath.filename();
			});
	}

	const FileNode* DirectoryTree::Internal_GetNode(
		const std::filesystem::path& path,
		const FileNode* node)
	{
		if (!node)
			return nullptr;

		if (std::filesystem::equivalent(node->info.filePath, path))
			return node;

		for (FileNode* child : node->children)
		{
			if (const FileNode* result = Internal_GetNode(path, child))
				return result;
		}

		return nullptr;
	}

	FileNode* DirectoryTree::GetRoot()
	{
		return root;
	}

	const FileNode* DirectoryTree::GetRoot() const
	{
		return root;
	}

	const FileNode* DirectoryTree::GetNode(const std::filesystem::path& path)
	{
		return Internal_GetNode(path, root);
	}
}