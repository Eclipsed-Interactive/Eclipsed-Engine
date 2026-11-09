#include "GameModuleManager.h"

#include "Core/BatchScript.h"
#include "Core/PathManager.h"
#include "EclipsedEngine/Plugin/PluginManager.h"

#include <Windows.h>
#include "Core/Standard/System/Process.h"

#include <EclipsedEngine/Scenes/SceneManager.h>
#include <Core/Logger/DebugLogger.h>

namespace Eclipse
{
	void* GameModuleManager::gameModule = nullptr;
	Func GameModuleManager::registerReflectedTypesFunc = nullptr;
	Func GameModuleManager::cleanupFunc = nullptr;

	std::string GameModuleManager::name;
	std::filesystem::path GameModuleManager::dllPath;

	bool GameModuleManager::TryCompileAndLoad()
	{
		if (!BuildDLL())
		{
			hasBuildErrors = false;
			return hasBuildErrors;
		}

		Load();

		hasBuildErrors = true;
		return hasBuildErrors;
	}

	bool GameModuleManager::Load()
	{
		const std::string dllString = dllPath.generic_string();
		gameModule = LoadLibraryA(dllString.c_str());
		if (!gameModule)
			return false;

		registerReflectedTypesFunc = (Func)GetProcAddress((HMODULE)gameModule, "RegisterReflectedTypes");
		if (!registerReflectedTypesFunc)
			return false;

		//cleanupFunc = (Func)GetProcAddress((HMODULE)gameModule, "Cleanup");
		//if (!cleanupFunc)
		//	return false;

		registerReflectedTypesFunc();

		return true;
	}

	void GameModuleManager::Unload()
	{
		if (!gameModule)
			return;

		FreeLibrary((HMODULE)gameModule);
		gameModule = nullptr;

		std::filesystem::remove(dllPath);

		registerReflectedTypesFunc = nullptr;
		cleanupFunc = nullptr;
	}

	bool GameModuleManager::Reload()
	{
		SceneManager::UnloadActive();
		const bool s = TryCompileAndLoad();
		SceneManager::LoadLastActive();

		hasBuildErrors = s;

		return s;
	}

	bool GameModuleManager::IsLoaded()
	{
		return gameModule != nullptr;
	}

	bool GameModuleManager::HasChanged()
	{
		namespace fs = std::filesystem;

		fs::path dllPath = "";

		if (!fs::exists(PathManager::GetProjectRoot() / "Bin"))
			return true;

		for (auto& entry : fs::recursive_directory_iterator(PathManager::GetProjectRoot() / "Bin"))
		{
			if (entry.path().extension().generic_string() == ".dll")
			{
				dllPath = entry.path();
				break;
			}
		}

		fs::file_time_type dllLastWriteTime = {};

		if (fs::exists(dllPath))
			dllLastWriteTime = fs::last_write_time(dllPath);


		for (auto& entry : fs::recursive_directory_iterator(PathManager::GetProjectRoot() / "Content"))
		{
			if (!entry.is_regular_file())
				continue;

			if (fs::last_write_time(entry.path()) > dllLastWriteTime)
				return true;
		}

		return false;
	}

	bool GameModuleManager::DllFileExists(const char* dllPath)
	{
		return std::filesystem::exists(dllPath);
	}

	bool GameModuleManager::RunBuildStep(const std::string& stepName, const std::filesystem::path& batchPath)
	{
		const std::filesystem::path logPath = PathManager::GetProjectRoot() / "Temp" / (stepName + ".log");

		std::error_code error;
		std::filesystem::create_directories(logPath.parent_path(), error);

		unsigned long exitCode = 0;
		if (Process::RunBatchAndWait(batchPath, logPath, exitCode))
			return true;

		Eclipse::Core::DebugLogger::AddGameLogMessage(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage("Game " + stepName + " failed (exit code " + std::to_string(exitCode) + "). Log: " + logPath.generic_string(), "C:\\Users\\zulto\\Desktop\\MyFiles\\Projects\\Eclipsed-Engine\\Source\\Editor\\Editor\\Game\\GameModuleManager.cpp", Eclipse::DiagnosticSeverity::Error), 138);;

		return false;
	}

	bool GameModuleManager::BuildDLL()
	{
		if (IsLoaded())
			Unload();

		bool wasChanged = HasChanged();

		if (wasChanged && !RunHeaderTool())
			return false;

		if (wasChanged && !GenerateCmake())
			return false;

		if (wasChanged && !Compile())
			return false;

		if (!CopyDllFile())
			return false;

		return true;
	}

	bool GameModuleManager::RunHeaderTool()
	{
		const std::string exePath = (PathManager::GetEngineRoot() / "HeaderTool.exe").generic_string();

		const std::string contentPath = (PathManager::GetProjectRoot() / "Content").generic_string();
		const std::string outputPath = (PathManager::GetProjectRoot() / "Generated" / "Reflection").generic_string();

		const char* args[] = {
			contentPath.c_str(),
			outputPath.c_str(),
			"false"
		};

		return Process::RunAndWait(exePath.c_str(), args, 3);
	}

	bool GameModuleManager::GenerateCmake()
	{
		const std::filesystem::path toolsPath = PathManager::GetProjectRoot() / "Tools";
		return RunBuildStep("generate", toolsPath / "generate.bat");
	}

	bool GameModuleManager::CopyDllFile()
	{
		std::filesystem::path oldDllPath = "";

		for (auto& entry : std::filesystem::recursive_directory_iterator(PathManager::GetProjectRoot() / "Bin"))
		{
			if (entry.path().extension().generic_string() == ".dll")
			{
				oldDllPath = entry.path();
				break;
			}
		}

		dllPath = PathManager::GetEngineRoot() / oldDllPath.filename();
		if (!std::filesystem::exists(oldDllPath))
			return false;

		return std::filesystem::copy_file(oldDllPath, dllPath, std::filesystem::copy_options::overwrite_existing);
	}

	bool GameModuleManager::Compile()
	{
		if (std::filesystem::exists(PathManager::GetProjectRoot() / "Bin"))
		{
			for (auto& entry : std::filesystem::recursive_directory_iterator(PathManager::GetProjectRoot() / "Bin"))
			{
				if (entry.path().extension().generic_string() == ".dll")
				{
					std::filesystem::path pdb = entry.path();
					pdb.replace_extension(".pdb");

					std::filesystem::remove(entry.path());
					std::filesystem::remove(pdb);
					break;
				}
			}
		}

		const std::filesystem::path toolsPath = PathManager::GetProjectRoot() / "Tools";
		return RunBuildStep("compile", toolsPath / "compile.bat");
	}
}

