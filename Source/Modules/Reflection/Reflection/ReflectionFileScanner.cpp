#include "ReflectionFileScanner.h"

#include <iostream>eesssadfc
#include <regex>
#include <string>
#include <fstream>
#include <filesystem>

#include "TypeDescriptor.h"
#include "TypeRegistry.h"

namespace Eclipse::Reflection
{
	void ReflectionFileScanner::ScanAndReflectFiles(const char* root)
	{
		for (auto it : std::filesystem::recursive_directory_iterator(root))
		{
			const std::string extension = it.path().extension().string();

			if (it.is_directory()) continue;
			else if (extension != ".h" && extension != ".hpp") continue;

			const std::string path = it.path().generic_string();
			ReflectFile(path.c_str());
		}
	}

	void ReflectionFileScanner::ReflectFile(const char* path)
	{
		std::string source = ReadFile(path);

		const std::regex fieldsRegex(
			R"(\[\[([\s\S]*?)\]\]\s*(?:(inline)\s+)?(.+?)\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?:\(|=|;))"
		);

		const std::regex attributesRegex(
			R"(\s*([A-Za-z_][A-Za-z0-9_]*)(?:\((.*?)\))?\s*(?:,|$))"
		);

		const std::regex typeRegex(
			R"(\b(?:class|struct)\s+(?:[A-Za-z_][A-Za-z0-9_]*\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*([\s\S]*))"
		);

		auto begin = std::sregex_iterator(
			source.begin(),
			source.end(),
			typeRegex
		);

		auto typeEnd = std::sregex_iterator();
		for (auto typeIt = begin; typeIt != typeEnd; typeIt++)
		{
			const std::smatch& match = *typeIt;

			const std::string name = match[1];

			TypeDescriptor type;
			type.Name = name;
			type.FilePath = path;

			const std::string body = match[2];
			auto fieldsBegin = std::sregex_iterator(
				body.begin(),
				body.end(),
				fieldsRegex
			);

			auto fieldsEnd = std::sregex_iterator();
			for (auto fieldsIt = fieldsBegin; fieldsIt != fieldsEnd; fieldsIt++)
			{
				const std::smatch& fieldMatch = *fieldsIt;

				FieldDescriptor field;
				field.Name = fieldMatch[4];
				field.Type = fieldMatch[3];

				const std::string attributes = fieldMatch[1];
				auto attributesBegin = std::sregex_iterator(
					attributes.begin(),
					attributes.end(),
					attributesRegex
				);

				auto attribtuesEnd = std::sregex_iterator();
				for (auto attributesIt = attributesBegin; attributesIt != attribtuesEnd; attributesIt++)
				{
					const std::smatch& attributesMatch = *attributesIt;

					const std::string attributeName = attributesMatch[1];
					const std::string attributeArgs = attributesMatch[2];

					field.attributes.emplace_back(attributeName, attributeArgs);
				}

				type.Fields.emplace_back(std::move(field));
			}

			TypeRegistry::Register(type);
		}
	}

	std::string ReflectionFileScanner::ReadFile(const char* path)
	{
		std::ifstream file(path, std::ios::binary);

		if (!file)
			return "";

		return std::string(
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()
		);
	}
}