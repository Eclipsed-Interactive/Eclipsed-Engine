#include "AssetPath.h"

#include <filesystem>

namespace Eclipse::AssetSystem
{
	AssetPath::AssetPath(const char* aScheme, const std::filesystem::path& aRelativePath)
		: myScheme(aScheme)
	{
		myFileName = aRelativePath.filename().generic_string();
		myRelativePath = aRelativePath.parent_path().generic_string();

		myVirtualPath = myScheme + "://" + myRelativePath + "/" + myFileName;
	}

	AssetPath::AssetPath(const char* aVirtualPath)
	{
		SetVirtualPath(aVirtualPath);
	}

	void AssetPath::SetVirtualPath(const char* aVirtualPath)
	{
		const std::string path = aVirtualPath;
		const size_t separator = path.find('://');

		if (separator == std::string::npos) return;

		myVirtualPath = aVirtualPath;

		myScheme = path.substr(0, separator);

		std::filesystem::path relative = path.substr(separator);
		myRelativePath = relative.parent_path().generic_string();
		myFileName = relative.filename().generic_string();
	}

	const std::string& AssetPath::Get() const
	{
		return myVirtualPath;
	}

	const std::string& AssetPath::Path() const
	{
		return myRelativePath;
	}

	const std::string& AssetPath::Scheme() const
	{
		return myScheme;
	}

	const std::string& AssetPath::FileName() const
	{
		return myFileName;
	}
}