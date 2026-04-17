#pragma once

#include "rapidjson/document.h"

namespace Eclipse
{
	template<typename T>
	struct JsonSerializer
	{
		static rapidjson::Value ToJsonImpl(const T& aValue, rapidjson::Document::AllocatorType& alloc) { };
		static T FromJsonImpl(const rapidjson::Value& aValue) {};
	};

	template<typename U>
	struct JsonData
	{
	private:
		using CleanT = std::remove_cvref_t<U>;

	public:
		static rapidjson::Value ToJson(const U& aValue, rapidjson::Document::AllocatorType& alloc)
		{
			CleanT newVal = aValue;
			return JsonSerializer<CleanT>::ToJsonImpl(newVal, alloc);
		};

		static U FromJson(const rapidjson::Value& aValue) {};
	};
}

#include "JsonSerializer.inl"