#pragma once

#include <string>
#include <any>
#include <unordered_map>
#include "rapidjson/document.h"

#include "Core/Math/Vector/Vector2.h"

#include "Core.Core.hpp"

namespace Eclipse::Configs
{
	class CORE_API Config
	{
	public:
		template<typename T>
		void Add(const char* key, const T& defaultValue);

		void Save(const char* path);
		static Config Load(const char* path);

		template<typename T>
		T Get(const char* key);

		void GenerateHeaderFile();

	private:
		template<typename T>
		void AddToJson(const std::any& val, rapidjson::Document& doc, rapidjson::Value& obj, const std::string& name);

		template<typename T>
		void AddVectorToJson(rapidjson::Value& value, const std::any& val, rapidjson::Document::AllocatorType& allocator);

		template<typename T>
		void LoadVectorFromJson(const std::string& name, const rapidjson::Value& value);

	private:
		std::string name = "BuildSettings";
		std::unordered_map<std::string, std::any> values;
	};
}

#include "Config.inl"