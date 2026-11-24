#include "SceneImporter.h"

#include "AssetSystem/Import/ImportContext.h"
#include "Core/Helpers/STB_Helper.h"

namespace Eclipse::AssetSystem
{
	bool SceneImporter::Import(ImportContext& ctx) const
	{
		ArtifactWriter& writer = ctx.Out();

		std::ifstream in(ctx.GetPath(), std::ios::binary);
		if (!in) return false;

		std::stringstream ss;
		ss << in.rdbuf();
		const std::string json = ss.str();

		const Size size = json.size();
		writer.Write(&size, sizeof(Size));
		writer.Write(json.c_str(), json.size());

		return true;
	}
}