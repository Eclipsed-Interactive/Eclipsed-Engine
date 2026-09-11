#include "ReflectionGenerator.h"

#include "TypeRegistry.h"
#include "TypeDescriptor.h"

#include <fstream>
#include <filesystem>

namespace Eclipse::Reflection
{
	void ReflectionGenerator::Clear()
	{
		std::filesystem::remove_all("Generated/");
	}

	void Eclipse::Reflection::ReflectionGenerator::Generate()
	{
		std::filesystem::create_directory("Generated");

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			GenerateFile(type);
		}

		GenerateRegistratorFile();
	}

	void ReflectionGenerator::GenerateFile(const TypeDescriptor& descriptor)
	{
		std::ofstream file(
			"Generated/" + descriptor.Name + ".ref.hpp",
			std::ios::binary | std::ios::trunc
		);

		if (!file)
			throw std::runtime_error("Failed to open reflection file");

		file
			<< "#pragma once\n\n"
			<< "#include \"Reflection/ReflectionRegistry.h\"\n\n"
			<< "using namespace Eclipse;\n"
			<< "using namespace Eclipse::Reflection;\n"
			<< "using namespace Eclipse::Game;\n\n";

		file
			<< "struct " << descriptor.Name << "_Reflection\n"
			<< "{\n"
			<< "\tstatic void Register()\n"
			<< "\t{\n";

		for (const auto& field : descriptor.Fields)
		{
			file
				<< "\t\tReflection::ReflectionRegistry::RegisterField(\n"
				<< "\t\t\t\"" << descriptor.Name << "\",\n"
				<< "\t\t\t\"" << field.Name << "\",\n"
				<< "\t\t\toffsetof(" << descriptor.Name << ", " << field.Name << ")\n"
				<< "\t\t);\n\n";
		}

		file
			<< "\t}\n"
			<< "};\n";
	}

	void ReflectionGenerator::GenerateRegistratorFile()
	{
		std::ofstream file(
			"Generated/ReflectionRegistrator.ref.hpp",
			std::ios::binary | std::ios::trunc
		);

		if (!file)
			throw std::runtime_error("Failed to open reflection registrator file");

		file << "#pragma once\n\n";

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			file << "#include \"" << type.FilePath << "\"\n";
		}

		file << "\n";

		file <<
			"extern \"C\"\n"
			"{\n"
			"\tinline void RegisterReflectedTypes()\n"
			"\t{\n";

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			file << "\t\t" << type.Name << "_Reflection::Register();\n";
		}

		file << "\t}\n}\n";
	}
}