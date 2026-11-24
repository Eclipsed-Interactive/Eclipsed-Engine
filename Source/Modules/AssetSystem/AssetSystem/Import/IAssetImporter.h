#pragma once

namespace Eclipse::AssetSystem
{
	class ImportContext;
	class IAssetImporter
	{
	public:
		virtual bool Import(ImportContext& ctx) const = 0;

		virtual constexpr unsigned int GetMagic() const = 0;
	};
}