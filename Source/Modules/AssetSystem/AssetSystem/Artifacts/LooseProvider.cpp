#include "LooseProvider.h"

namespace Eclipse::AssetSystem
{
	ArtifactIndex& LooseProvider::Index()
	{
		return index;
	}

	bool LooseProvider::Contains(const GUID& aGuid) const
	{
		return false;
	}
	
	bool LooseProvider::Read(const GUID& aGuid, std::vector<Byte>& outBytes) const
	{
		if (!index.TryGetPath(aGuid)) return false;

		const std::string guidStr = aGuid.ToString();
		const std::filesystem::path file = artifactLocation / guidStr.substr(0, 2) / guidStr;

		std::ifstream in(file, std::ios::binary | std::ios::ate);
		if (!in) return false;

		const std::streamsize size = in.tellg();
		if (size <= 0) return false;

		outBytes.resize(size);
		in.seekg(0, std::ios::beg);

		in.read(reinterpret_cast<char*>(outBytes.data()), size);

		return true;
	}

	bool LooseProvider::TryGetGuid(const std::string& aVirtualPath, GUID& outGuid) const
	{
		return index.TryGetGuid(aVirtualPath, outGuid);
	}
}