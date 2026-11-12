
#include "ReflectionFileScanner.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "TypeDescriptor.h"
#include "TypeRegistry.h"

namespace Eclipse::Reflection
{
	namespace
	{
		static  std::regex typeRegex(
			R"(\b(?:class|struct)\s+(?:[A-Za-z_][A-Za-z0-9_]*\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*(?:final\s*)?(?::[^;{}]*)?\{)"
		);

		const std::regex fieldsRegex(
			R"(\[\[([^\]]*)\]\]\s*(?:(inline)\s+)?([^;{}()=\[\]]+?)\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?=[=;{]))"
		);

		const std::regex attributesRegex(
			R"(\s*([A-Za-z_][A-Za-z0-9_]*(?:::[A-Za-z_][A-Za-z0-9_]*)*)(?:\s*\((.*?)\))?\s*(?:,|$))"
		);

		std::string MaskNonCode(const std::string& source)
		{
			std::string result = source;

			const auto mask = [&](std::size_t start, std::size_t end)
				{
					for (std::size_t i = start; i < end; ++i)
					{
						if (result[i] != '\n' && result[i] != '\r')
							result[i] = ' ';
					}
				};

			for (std::size_t i = 0; i < source.size();)
			{
				if (source.compare(i, 2, "//") == 0)
				{
					const auto end = source.find('\n', i);
					const auto next = end == std::string::npos
						? source.size()
						: end;

					mask(i, next);
					i = next;
					continue;
				}

				if (source.compare(i, 2, "/*") == 0)
				{
					const auto end = source.find("*/", i + 2);
					const auto next = end == std::string::npos
						? source.size()
						: end + 2;

					mask(i, next);
					i = next;
					continue;
				}

				if (source.compare(i, 2, "R\"") == 0)
				{
					const auto opening = source.find('(', i + 2);

					if (opening != std::string::npos &&
						opening - (i + 2) <= 16)
					{
						const std::string delimiter = source.substr(
							i + 2,
							opening - (i + 2)
						);

						const std::string closing =
							")" + delimiter + "\"";

						const auto end = source.find(
							closing,
							opening + 1
						);

						const auto next = end == std::string::npos
							? source.size()
							: end + closing.size();

						mask(i, next);
						i = next;
						continue;
					}
				}

				if (source[i] == '"' || source[i] == '\'')
				{
					if (source[i] == '\'' && i > 0 &&
						i + 1 < source.size() &&
						source[i - 1] >= '0' && source[i - 1] <= '9' &&
						source[i + 1] >= '0' && source[i + 1] <= '9')
					{
						++i;
						continue;
					}

					const auto start = i;
					const char quote = source[i++];

					while (i < source.size())
					{
						if (source[i] == '\\')
						{
							++i;

							if (i < source.size())
								++i;
						}
						else if (source[i++] == quote)
						{
							break;
						}
					}

					mask(start, i);
					continue;
				}

				++i;
			}

			return result;
		}

		std::string Trim(const std::string& text)
		{
			const auto first = text.find_first_not_of(" \t\r\n");

			if (first == std::string::npos)
				return {};

			const auto last = text.find_last_not_of(" \t\r\n");
			return text.substr(first, last - first + 1);
		}
	}

	void ReflectionFileScanner::ScanAndReflectFiles(const std::filesystem::path& root)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
		{
			if (!entry.is_regular_file())
				continue;

			const std::string extension = entry.path().extension().string();

			if (extension == ".h" || extension == ".hpp")
				ReflectFile(entry.path());
		}
	}

	void ReflectionFileScanner::ReflectFile(const std::filesystem::path& path)
	{
		const std::string original = ReadFile(path);
		const std::string source = MaskNonCode(original);

		const auto typesBegin = std::sregex_iterator(source.begin(), source.end(), typeRegex);
		for (auto typeIt = typesBegin; typeIt != std::sregex_iterator(); ++typeIt)
		{
			const std::smatch& match = *typeIt;

			const size_t bodyStart = match.position() + match.length();
			ReflectionFlags flags;

			size_t bodyEnd = FindBodyEnd(source, bodyStart);
			if (bodyEnd == 0) continue;

			const size_t bodySize = bodyEnd - bodyStart - 1;
			const std::string body = source.substr(bodyStart, bodySize);

			if (body.find("MAKE_REFLECTABLE") == std::string::npos)
				continue;

			TypeDescriptor type = ConstructType(match[1].str(), path, FindReflectionFlag(body));
			FindFields(type, body, original, bodyStart);

			TypeRegistry::Register(type);
		}
	}

	std::string ReflectionFileScanner::ReadFile(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::binary);

		if (!file)
		{
			std::cerr << "Could not read " << path << '\n';
			return {};
		}

		return std::string(
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()
		);
	}

	void ReflectionFileScanner::CheckDepth(const std::string& abody, std::vector<int>* outDepths)
	{
		size_t depth = 0;
		for (std::size_t i = 0; i < abody.size(); ++i)
		{
			if (outDepths) (*outDepths)[i] = depth;

			if (abody[i] == '{')
				++depth;
			else if (abody[i] == '}' && depth > 0)
				--depth;
		}
	}

	size_t ReflectionFileScanner::FindBodyEnd(const std::string& aSource, size_t aBodyStart)
	{
		size_t bodyEnd = aBodyStart;
		size_t depth = 1;

		while (bodyEnd < aSource.size() && depth > 0)
		{
			if (aSource[bodyEnd] == '{')
				++depth;
			else if (aSource[bodyEnd] == '}')
				--depth;

			++bodyEnd;
		}

		if (depth != 0)
		{
			std::cerr << "Unclosed class or struct in " << aSource << '\n';
			return 0;
		}

		return bodyEnd;
	}

	void ReflectionFileScanner::FindFields(TypeDescriptor& aType, const std::string& aBody, const std::string& aOriginal, size_t aBodyStart)
	{
		std::vector<int> fieldDepths(aBody.size());
		CheckDepth(aBody, &fieldDepths);

		const std::sregex_iterator fieldsBegin = std::sregex_iterator(aBody.begin(), aBody.end(), fieldsRegex);

		for (auto fieldIt = fieldsBegin; fieldIt != std::sregex_iterator(); ++fieldIt)
		{
			const std::smatch& fieldMatch = *fieldIt;

			const size_t fieldPosition = fieldMatch.position();

			if (fieldDepths[fieldPosition] != 0)
				continue;

			FieldDescriptor field;
			field.Name = fieldMatch[4].str();
			field.Type = Trim(fieldMatch[3].str());

			if (field.Type.empty())
				continue;

			const std::string attributes = aOriginal.substr(aBodyStart + fieldMatch.position(1), fieldMatch.length(1));
			AddAttributesToField(attributes, field);

			if (!field.attributes.empty()) // Only adds the field to the type if there are attributes.
				aType.Fields.emplace_back(std::move(field));
		}
	}

	ReflectionFlags ReflectionFileScanner::FindReflectionFlag(const std::string& aBody)
	{
		size_t start = aBody.find("REFLECTION_FLAG");
		if (start != std::string::npos)
		{
			start += std::string("REFLECTION_FLAG(").length();
			size_t end = aBody.find(')', start);

			if (end != std::string::npos)
			{
				std::string flag = aBody.substr(start, end - start);
				std::transform(flag.begin(), flag.end(), flag.begin(),
					[](unsigned char c) {
						return static_cast<char>(std::toupper(c));
					});

				if (flag == "COMPONENT")
					return ReflectionFlags::Component;
				else if (flag == "SERIALIZABLE")
					return ReflectionFlags::Serializable;
			}
		}
		else return ReflectionFlags::None;
	}

	TypeDescriptor ReflectionFileScanner::ConstructType(const std::string& aTypeName, const std::filesystem::path& aSourceFilePath, ReflectionFlags aReflectionFlag)
	{
		TypeDescriptor type;
		type.Name = aTypeName;
		type.FilePath = aSourceFilePath.generic_string();
		type.flag = aReflectionFlag;

		return type;
	}

	void ReflectionFileScanner::AddAttributesToField(const std::string& aAttributes, FieldDescriptor& aField)
	{
		const std::sregex_iterator attributesBegin = std::sregex_iterator(aAttributes.begin(), aAttributes.end(), attributesRegex);
		for (auto attributeIt = attributesBegin;attributeIt != std::sregex_iterator();++attributeIt)
		{
			const std::smatch& attributeMatch = *attributeIt;

			aField.attributes.emplace_back(attributeMatch[1].str(), attributeMatch[2].str());
		}
	}
}
