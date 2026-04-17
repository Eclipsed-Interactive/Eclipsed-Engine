#pragma once

#include <string>
#include "rapidjson/document.h"

namespace Eclipse::Editor
{
	struct WindowData {
		int id;
		std::string name;

	public:
		static rapidjson::Value Serialize(const WindowData& aVec, rapidjson::Document::AllocatorType& alloc);
		static WindowData Deserialize(const rapidjson::Value& aValue);
	};
}