#include "AssetImporter.h"

#include "IAssetImporter.h"
#include "ImporterRegistry.h"
#include <AssetSystem/MetaData/AssetMeta.h>
#include <AssetSystem/MetaData/MetaSerializer.h>
#include <AssetSystem/Import/ImportContext.h>
#include <AssetSystem/SourceMount.hpp>

#include "ArtifactHeader.h"

#include <AssetSystem/AssetCache.h>

namespace Eclipse::AssetSystem
{
	void AssetImporterd::Import(const SourceMount& aMount, const AssetMeta* aMeta)
	{
		if (!std::filesystem::is_regular_file(aMeta->SourcePath())) return;

		const std::string extension = aMeta->SourcePath().extension().string();
		if (extension == ".meta") return;

		const IAssetImporter* importer = ImporterRegistry::FindByExtension(extension);
		if (importer == nullptr) return;

		ImportContext ctx(aMeta);

		if (!importer->Import(ctx))
		{
			return;
		}

		ArtifactHeader header;
		{
			header.magic = importer->GetMagic();
			header.payloadSize = ctx.Out().GetBytes().size();
		}

		ArtifactWriter writer;
		writer.Write(&header, sizeof(ArtifactHeader));
		writer.Write(ctx.Out().GetBytes().data(), header.payloadSize);

		writer.SaveToFile(aMeta->ArtifactPath());

		AssetData* data = AssetCache::Get(aMeta->GUID());
		if(data != nullptr)
			AssetCache::MarkForReload(aMeta->GUID());
	}
}