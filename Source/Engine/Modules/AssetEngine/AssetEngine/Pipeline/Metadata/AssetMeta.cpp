#include "AssetMeta.h"

#include "AssetEngine/SupportedAssets.h"

#include "Data/TextureMeta.h"
#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "CoreEngine/PathManager.h"

namespace Eclipse::Assets
{
	AssetMeta::~AssetMeta()
	{
		for (auto& [type, metaComp] : metaComponents)
		{
			delete metaComp;
		}
	}

	void AssetMeta::WriteToStream(std::ofstream& _of) const
	{
		cereal::JSONOutputArchive ar(_of);
		ar(*this);
	}

	void AssetMeta::LoadFromStream(std::ifstream& _if)
	{
		cereal::JSONInputArchive ar(_if);
		ar(*this);
	}

	void AssetMeta::WriteToStreamBinary(std::ofstream& _of) const
	{
		cereal::BinaryOutputArchive ar(_of);
		ar(*this);


		//const std::string guidString = guid.ToString();
		//_of.write(reinterpret_cast<const char*>(guidString.data()), guidString.size());
		//_of.write(reinterpret_cast<const char*>(&type), sizeof(AssetType));

		//for (const auto& [type, metaComp] : metaComponents)
		//{
		//	metaComp->WriteToStream(_of);
		//}
	}

	void AssetMeta::LoadFromStreamBinary(std::ifstream& _if)
	{
		cereal::BinaryInputArchive ar(_if);
		ar(*this);

		//std::string guidString = "";
		//guidString.resize(32);
		//_if.read(reinterpret_cast<char*>(guidString.data()), 32);
		//_if.read(reinterpret_cast<char*>(&type), sizeof(AssetType));

		//guid.FromString(guidString);

		//for (const auto& [type, metaComp] : metaComponents)
		//{
		//	metaComp->LoadFromStream(_if);
		//}
	}
	std::string Assets::AssetMeta::GetArtifactPath() const
	{
		std::string guidString = guid.ToString();
		const std::filesystem::path filePath = PathManager::GetArtifactsPath() / guidString.substr(0, 2) / guidString;

		return filePath.generic_string();
	}
}