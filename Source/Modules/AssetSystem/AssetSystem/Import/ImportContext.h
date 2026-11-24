#pragma once

#include <MetaData/AssetMeta.h>
#include <AssetSystem/Import/ArtifactWriter.h>

namespace Eclipse::AssetSystem
{
	class ImportContext
	{
	public:
		ImportContext(const AssetMeta* aMeta) : myMeta(aMeta) {}

	public:
		[[nodiscard]] const std::filesystem::path& GetPath() const { return myMeta->SourcePath(); }
		[[nodiscard]] ArtifactWriter& Out() { return myOut; }
		[[nodiscard]] const AssetMeta* Meta() { return myMeta; }

	private:
		const AssetMeta* myMeta;
		ArtifactWriter myOut;
	};
}