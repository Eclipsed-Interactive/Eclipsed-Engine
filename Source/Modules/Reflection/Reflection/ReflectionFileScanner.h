#pragma once

#include <filesystem>
#include <string>
#include <regex>
#include "Reflection.Core.hpp"

#include "FieldEntry.h"
#include "TypeEntry.h"
#include "FieldDescriptor.h"
#include "Runtime/ReflectionFlags.h"

namespace Eclipse::Reflection
{
	class REFLECTION_API ReflectionFileScanner final 
	{
	public:
		static void ScanAndReflectFiles(const std::filesystem::path& root);
		static void ReflectFile(const std::filesystem::path& root);

	private:
		static std::string ReadFile(const std::filesystem::path& root);

		/// <summary>
		/// Checks the depths of each character in the class. '{' and '}' increments and decrements the depths.
		/// </summary>
		/// <param name="aBody">The class body.</param>
		/// <param name="outDepths">The depths of each character.</param>
		static void CheckDepth(const std::string& aBody, std::vector<int>* outDepths = nullptr);

		/// <summary>
		/// Finds the end of a body.
		/// </summary>
		/// <param name="aSource">The source files content.</param>
		/// <param name="aBodyStart">The start of the body. '{'</param>
		/// <returns>Where the body ends if successfull, 0 if unsuccessfull.</returns>
		static size_t FindBodyEnd(const std::string& aSource, size_t aBodyStart);

		static void FindFields(TypeDescriptor& aType, const std::string& aBody, const std::string& aOriginal, size_t aBodyStart);


		static ReflectionFlags FindReflectionFlag(const std::string& aBody);

		static TypeDescriptor ConstructType(const std::string& aTypeName, const std::filesystem::path& aSourceFilePath, ReflectionFlags aReflectionFlag);

		static void AddAttributesToField(const std::string& aAttributes, FieldDescriptor& aField);
	};
}