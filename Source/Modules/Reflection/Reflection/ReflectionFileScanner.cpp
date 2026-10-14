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
                    // Preserve common numeric digit separators.
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

    void ReflectionFileScanner::ScanAndReflectFiles(
        const std::filesystem::path& root)
    {
        for (const auto& entry :
            std::filesystem::recursive_directory_iterator(root))
        {
            if (!entry.is_regular_file())
                continue;

            const std::string extension =
                entry.path().extension().string();

            if (extension == ".h" || extension == ".hpp")
                ReflectFile(entry.path());
        }
    }

    void ReflectionFileScanner::ReflectFile(
        const std::filesystem::path& path)
    {
        const std::string original = ReadFile(path);
        const std::string source = MaskNonCode(original);

        const std::regex typeRegex(
            R"(\b(?:class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?:final\s*)?(?::[^;{}]*)?\{)"
        );

        const std::regex fieldsRegex(
            R"(\[\[([^\]]*)\]\]\s*(?:(inline)\s+)?([^;{}()=\[\]]+?)\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?=[=;{]))"
        );

        const std::regex attributesRegex(
            R"(\s*([A-Za-z_][A-Za-z0-9_]*(?:::[A-Za-z_][A-Za-z0-9_]*)*)(?:\s*\((.*?)\))?\s*(?:,|$))"
        );

        const auto typesBegin = std::sregex_iterator(
            source.begin(),
            source.end(),
            typeRegex
        );

        for (auto typeIt = typesBegin;
            typeIt != std::sregex_iterator();
            ++typeIt)
        {
            const std::smatch& match = *typeIt;

            const auto bodyStart =
                static_cast<std::size_t>(match.position()) +
                static_cast<std::size_t>(match.length());

            std::size_t bodyEnd = bodyStart;
            std::size_t depth = 1;

            while (bodyEnd < source.size() && depth > 0)
            {
                if (source[bodyEnd] == '{')
                    ++depth;
                else if (source[bodyEnd] == '}')
                    --depth;

                ++bodyEnd;
            }

            if (depth != 0)
            {
                std::cerr << "Unclosed class or struct in "
                    << path << '\n';
                continue;
            }

            const std::string body = source.substr(
                bodyStart,
                bodyEnd - bodyStart - 1
            );

            TypeDescriptor type;
            type.Name = match[1].str();
            type.FilePath = path.generic_string();

            // Only fields directly inside this class are considered.
            std::vector<std::size_t> fieldDepths(body.size());
            depth = 0;

            for (std::size_t i = 0; i < body.size(); ++i)
            {
                fieldDepths[i] = depth;

                if (body[i] == '{')
                    ++depth;
                else if (body[i] == '}' && depth > 0)
                    --depth;
            }

            const auto fieldsBegin = std::sregex_iterator(
                body.begin(),
                body.end(),
                fieldsRegex
            );

            for (auto fieldIt = fieldsBegin;
                fieldIt != std::sregex_iterator();
                ++fieldIt)
            {
                const std::smatch& fieldMatch = *fieldIt;

                const auto fieldPosition =
                    static_cast<std::size_t>(fieldMatch.position());

                if (fieldDepths[fieldPosition] != 0)
                    continue;

                FieldDescriptor field;
                field.Name = fieldMatch[4].str();
                field.Type = Trim(fieldMatch[3].str());

                if (field.Type.empty())
                    continue;

                // Recover original attribute arguments, including strings.
                const auto attributesPosition = bodyStart +
                    static_cast<std::size_t>(fieldMatch.position(1));

                const std::string attributes = original.substr(
                    attributesPosition,
                    static_cast<std::size_t>(fieldMatch.length(1))
                );

                const auto attributesBegin = std::sregex_iterator(
                    attributes.begin(),
                    attributes.end(),
                    attributesRegex
                );

                for (auto attributeIt = attributesBegin;
                    attributeIt != std::sregex_iterator();
                    ++attributeIt)
                {
                    const std::smatch& attributeMatch = *attributeIt;

                    field.attributes.emplace_back(
                        attributeMatch[1].str(),
                        attributeMatch[2].str()
                    );
                }

                if (!field.attributes.empty())
                    type.Fields.emplace_back(std::move(field));
            }

            // Register once, and only when attributed fields were found.
            if (!type.Fields.empty())
                TypeRegistry::Register(type);
        }
    }

    std::string ReflectionFileScanner::ReadFile(
        const std::filesystem::path& path)
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
}