#include "ReflectionGenerator.h"

#include "TypeRegistry.h"
#include "TypeDescriptor.h"

#include <fstream>
#include <filesystem>

#include "Core/PathManager.h"

namespace Eclipse::Reflection
{
	void ReflectionGenerator::Clear(const std::filesystem::path& path)
	{
		std::filesystem::remove_all(path);
	}

	void Eclipse::Reflection::ReflectionGenerator::Generate(const std::filesystem::path& path)
	{
		std::filesystem::create_directory(PathManager::GetProjectRoot() / "Generated" / "Reflection");

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			GenerateFile(type);
		}

		GenerateRegistratorFile();
	}

	void ReflectionGenerator::GenerateFile(const TypeDescriptor& descriptor)
	{
		using namespace std::filesystem;

		const path filepath = PathManager::GetProjectRoot() / "Generated" / "Reflection" / std::string(descriptor.Name + ".ref.hpp");
		std::ofstream file(filepath, std::ios::binary | std::ios::trunc);

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
		using namespace std::filesystem;

		const path filepath = PathManager::GetProjectRoot() / "Generated" / "Reflection" / "ReflectionRegistrator.ref.hpp";
		std::ofstream file(filepath, std::ios::binary | std::ios::trunc);

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