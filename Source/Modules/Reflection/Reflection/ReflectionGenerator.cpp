#include "ReflectionGenerator.h"

#include "TypeRegistry.h"
#include "TypeDescriptor.h"

#include <fstream>
#include <filesystem>
#include <Windows.h>
#include "Core/PathManager.h"

namespace Eclipse::Reflection
{
	void ReflectionGenerator::Clear(const std::filesystem::path& path)
	{
		std::error_code error;
		std::filesystem::remove_all(path, error);

		//MoveFileExW(path.c_str(), nullptr, MOVEFILE_DELAY_UNTIL_REBOOT);
	}

	void Eclipse::Reflection::ReflectionGenerator::Generate(const std::filesystem::path& path)
	{
		std::filesystem::create_directories(path);

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			GenerateFile(path, type);
		}

		GenerateRegistratorFile(path);
	}

	void ReflectionGenerator::GenerateFile(const std::filesystem::path& generationPath, const TypeDescriptor& descriptor)
	{
		using namespace std::filesystem;

		const path filepath = generationPath / std::string(descriptor.Name + ".ref.hpp");
		std::ofstream file(filepath, std::ios::binary | std::ios::trunc);

		if (!file)
			throw std::runtime_error("Failed to open reflection file");

		file
			<< "#pragma once\n"
			<< "\n"
			<< "#include <cstddef>\n"
			<< "#include \"Reflection/Runtime/ReflectionRegistry.h\"\n"
			<< "\n"
			<< "#include \"" << descriptor.FilePath << "\"\n"
			<< "\n"
			<< "using namespace Eclipse;\n"
			<< "using namespace Eclipse::Reflection;\n"
			<< "\n";

		file
			<< "inline void " << descriptor.Name << "_ReflectionRegister()\n"
			<< "{\n";

		file
			<< "\tReflection::ReflectionRegistry::RegisterType(\n"
			<< "\t\tTypeEntry{\n"
			<< "\t\t\t.name = \"" << descriptor.Name << "\", \n"
			<< "\t\t\t.size = sizeof(" << descriptor.Name << "),\n"
			<< "\t\t\t.id = " << descriptor.Name << "::GetID(),\n"
			<< "\t\t\t.create = [](){ return reinterpret_cast<void*>(new " << descriptor.Name << "); },\n"
			<< "\t\t\t.destroy = [](void* data){ delete reinterpret_cast<" << descriptor.Name << "*>(data); }\n"
			<< "\t\t}\n"
			<< "\t);\n"
			<< "\n";

		for (const auto& field : descriptor.Fields)
		{
			file
				<< "\tReflection::ReflectionRegistry::RegisterField(\n"
				<< "\t\t\"" << descriptor.Name << "\",\n"
				<< "\t\tFieldEntry\n"
				<< "\t\t{\n" 
				<< "\t\t\t.name = \"" << field.Name << "\",\n"
				<< "\t\t\t.offset = offsetof(" << descriptor.Name << ", " << field.Name << "),\n"
				<< "\t\t\t.size = sizeof(" << descriptor.Name << "::" << field.Name << ")\n"
				<< "\t\t}\n"
				<< "\t);\n"
				<< "\n";
		}

		file << "}\n";
	}

	void ReflectionGenerator::GenerateRegistratorFile(const std::filesystem::path& generationPath)
	{
		using namespace std::filesystem;

		const path filepath = generationPath / "ReflectionRegistrator.ref.hpp";
		std::ofstream file(filepath, std::ios::binary | std::ios::trunc);

		if (!file)
			throw std::runtime_error("Failed to open reflection registrator file");

		file << "#pragma once\n\n";

		file << "#include \"Plugin.Core.hpp\"\n\n";

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			file << "#include \"" << generationPath.generic_string() << "/" << type.Name << ".ref.hpp\"\n";
		}

		file << "\n";

		file <<
			"extern \"C\"\n"
			"{\n"
			"\tinline DLL void RegisterReflectedTypes()\n"
			"\t{\n";

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			file << "\t\t" << type.Name << "_ReflectionRegister();\n";
		}

		file << "\t}\n}\n";
	}
}