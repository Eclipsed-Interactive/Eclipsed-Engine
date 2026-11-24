#pragma once

#include "AssetMeta.h"

#include <string>
#include <vector>

#include "Core/Math/RectSizePos.h"

namespace Eclipse::AssetSystem
{
	struct Rect
	{
		Math::RectSizePos rect;
		std::string name;
	};

	struct ASSETSYSTEM_API TextureMeta : public AssetMeta
	{
	public:
		MOVE(TextureMeta);

	public:
		std::vector<Rect> spriteRects;

	protected:
		void SaveData(JsonWriter& writer) const override;
		void LoadData(const rapidjson::Value& data) override;
	};
}
