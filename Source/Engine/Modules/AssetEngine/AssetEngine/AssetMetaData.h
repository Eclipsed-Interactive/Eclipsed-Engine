#pragma once

#include "SupportedTypes.h"

#include "CoreEngine/Math/RectSizePos.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include "cereal/types/string.hpp"

namespace Eclipse::Assets
{
	struct AssetMetaData
	{
		size_t guid;
		AssetType type;

		std::string fileName;
		std::string fileExtension;
		std::string filePath;

		int width;
		int height;
		int channels;

		std::vector<Math::RectSizePos> spriteRects;

		AssetMetaData() = default;
		AssetMetaData(const char* aPath);
		virtual ~AssetMetaData();

		template <class Archive>
		void serialize(Archive& ar, const uint32_t version)
		{
			ar(
				CEREAL_NVP(guid),
				CEREAL_NVP(type),
				CEREAL_NVP(fileName),
				CEREAL_NVP(fileExtension),
				CEREAL_NVP(width),
				CEREAL_NVP(height),
				CEREAL_NVP(channels),
				CEREAL_NVP(spriteRects)
			);
		}

		static const AssetMetaData& GetDefaultErrorInstance();
	};

	//struct TextureMetaData : public AssetMetaData
	//{
	//    int width;
	//    int height;
	//    int channels;

	//    std::vector<Math::RectSizePos> spriteRects;


	//    template <class Archive>
	//    void serialize(Archive& ar, const uint32_t version)
	//    {
	//        ar(cereal::base_class<AssetMetaData>(this));
	//        ar(
	//            CEREAL_NVP(width),
	//            CEREAL_NVP(height),
	//            CEREAL_NVP(channels),
	//            CEREAL_NVP(spriteRects)
	//        );
	//    }
	//};
}
//
//CEREAL_REGISTER_TYPE(Eclipse::Assets::TextureMetaData);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Eclipse::Assets::AssetMetaData, Eclipse::Assets::TextureMetaData);

CEREAL_CLASS_VERSION(Eclipse::Assets::AssetMetaData, 1);
//CEREAL_CLASS_VERSION(Eclipse::Assets::TextureMetaData, 0);