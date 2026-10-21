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

	void ReflectionGenerator::Generate(const std::filesystem::path& path, bool useStatic)
	{
		std::filesystem::create_directories(path);

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			GenerateFile(path, type);
		}

		if (useStatic)
		{
			GenerateRegistratorFileStaticInitiator(path);
		}
		else
		{
			GenerateRegistratorFileDynamicInitiator(path);
		}
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
			<< "#include \"Reflection/Attributes/Attributes.hpp\"\n"
			<< "#include \"Reflection/ReflectionRegistrar.hpp\"\n"
			<< "#include \"Core/Standard/TypeIDGenerator.hpp\"\n"
			<< "\n"
			<< "#include \"" << descriptor.FilePath << "\"\n"
			<< "\n"
			<< "using namespace Eclipse;\n"
			<< "using namespace Eclipse::Reflection;\n"
			<< "\n";

		file
			<< "template<> struct ReflectionRegistrar<" << descriptor.Name << ">\n"
			<< "{\n";

		file
			<< "\tstatic inline void Register()\n"
			<< "\t{\n";



		file
			<< "\t\tReflection::ReflectionRegistry::RegisterType(\n"
			<< "\t\t\tTypeEntry{\n"
			<< "\t\t\t\t.name = \"" << descriptor.Name << "\", \n"
			<< "\t\t\t\t.size = sizeof(" << descriptor.Name << "),\n"
			<< "\t\t\t\t.id = Type::GetTypeID<" <<  descriptor.Name << ">(), \n"
			<< "\t\t\t\t.create = [](){ return reinterpret_cast<void*>(new " << descriptor.Name << "); },\n"
			<< "\t\t\t\t.createAtAddress = [](unsigned char* addr){ return reinterpret_cast<void*>(new (addr)" << descriptor.Name << "); },\n"
			<< "\t\t\t\t.destroy = [](void* data){ delete reinterpret_cast<" << descriptor.Name << "*>(data); },\n"
			<< "\t\t\t\t.flag = static_cast<Reflection::ReflectionFlags>("<< static_cast<int>(descriptor.flag) <<")\n"
			<< "\t\t\t}\n"
			<< "\t\t);\n"
			<< "\n";

		for (const auto& field : descriptor.Fields)
		{
			// Create a namepsace 
			file << "\t\t{\n";

			file
				<< "\t\t\tFieldEntry entry =\n"
				<< "\t\t\t{\n"
				<< "\t\t\t\t.name = \"" << field.Name << "\",\n"
				<< "\t\t\t\t.offset = offsetof(" << descriptor.Name << ", " << field.Name << "),\n"
				<< "\t\t\t\t.size = sizeof(" << descriptor.Name << "::" << field.Name << "),\n"
				<< "\t\t\t\t.type = Type::GetTypeID<decltype(" << descriptor.Name << "::" << field.Name << ")>()\n"
				<< "\t\t\t};\n"
				<< "\n";


			for (auto attr : field.attributes)
			{
				file << "\t\t\tentry.attributeContainer.Add<" << attr.Name << ">(" << attr.Arguments << ");\n";
			}


			file << "\n";

			file
				<< "\t\t\tReflection::ReflectionRegistry::RegisterField(\n"
				<< "\t\t\t\t\"" << descriptor.Name << "\",\n"
				<< "\t\t\t\tentry\n"
				<< "\t\t\t);\n";

			file << "\t\t}\n";
		}

		file << "\t}\n";

		file << "};\n";
	}

	void ReflectionGenerator::GenerateRegistratorFileStaticInitiator(const std::filesystem::path& generationPath)
	{
		using namespace std::filesystem;

		const path filepath = generationPath / "ReflectionRegistrator.ref.cpp";
		std::ofstream file(filepath, std::ios::binary | std::ios::trunc);

		if (!file)
			throw std::runtime_error("Failed to open reflection registrator file");

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			file << "#include \"" << generationPath.generic_string() << "/" << type.Name << ".ref.hpp\"\n";
		}

		file << "\n";

		file
			<< "struct ComponentsStaticInitiator\n"
			<< "{\n"
			<< "\tComponentsStaticInitiator()\n"
			<< "\t{\n";

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			file << "\t\tReflectionRegistrar<" << type.Name << ">::Register();";
			file << "\n";
		}

		file << "\n";

		file
			<< "\t}\n"
			<< "};\n"
			<< "\n"
			<< "inline ComponentsStaticInitiator __componentsStaticInititator = {};";
	}

	void ReflectionGenerator::GenerateRegistratorFileDynamicInitiator(const std::filesystem::path& generationPath)
	{
		using namespace std::filesystem;

		const path filepath = generationPath / "ReflectionRegistrator.ref.cpp";
		std::ofstream file(filepath, std::ios::binary | std::ios::trunc);

		if (!file)
			throw std::runtime_error("Failed to open reflection registrator file");

		file << "#pragma once\n\n";

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			file << "#include \"" << generationPath.generic_string() << "/" << type.Name << ".ref.hpp\"\n";
		}

		file << "\n";

		file <<
			"extern \"C\"\n"
			"{\n"
			"\tinline __declspec(dllexport) void RegisterReflectedTypes()\n"
			"\t{\n";

		for (auto [id, type] : TypeRegistry::GetAll())
		{
			file << "\t\tReflectionRegistrar<" << type.Name << ">::Register();";
			file << "\n";
		}

		file << "\t}\n}\n";
	}
}