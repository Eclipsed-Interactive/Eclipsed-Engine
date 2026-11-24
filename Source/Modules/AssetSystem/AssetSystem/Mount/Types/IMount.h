#pragma once

#include <filesystem>
#include <Core/GUID.h>

namespace Eclipse::AssetSystem
{
	class IMount
	{
	public:
		IMount(const std::string& aScheme, const std::string& aPath)
			: myScheme(aScheme), myPath(aPath) {}

	public:
		virtual bool Exists(const std::string& aScheme, const std::string& aPath);
		virtual bool Exists(const std::filesystem::path& aPath);
		virtual bool Exists(GUID aGuid);

		[[nodiscard]] const std::string& Path() const { return myPath; }
		[[nodiscard]] const std::string& Scheme() const { return myScheme; }

	private:
		std::string myScheme;
		std::string myPath;
	};
}