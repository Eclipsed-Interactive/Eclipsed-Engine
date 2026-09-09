//#include "GameCompiler.h"
//
//#include "EclipsedEngine/Core/PathManager.h"
//#include "GameLoader.h"
//#include <filesystem>
//
//#include "EclipsedEngine/Scenes/SceneManager.h"
//
//#include "EclipsedEngine/Core/Settings/BuildSettings.h"
//
//namespace Eclipse
//{
//	void GameModuleManager::GenerateGameEditor()
//	{
//		return;
//
//#ifdef _DEBUG
//		const bool isDebugBuild = true;
//#else
//		const bool isDebugBuild = false;
//#endif
//
//		Settings::BuildSettings::Data& data = Settings::BuildSettings::GetData();
//
//		static BatchScript script(
//			PathManager::GetProjectRoot() / "Tools",
//			"generate-game-editor.bat",
//			{
//				"\"PROJECT_DIR=" + PathManager::GetProjectRoot().generic_string() + "\"",
//				"\"ENGINE_PATH=" + PathManager::GetEngineRoot().parent_path().generic_string() + "\"",
//				"\"CONFIG=" + std::string(isDebugBuild ? "Debug" : "Release") + "\"",
//				"\"ENABLE_NETWORKING=" + std::string(data.enableNetworking ? "ON" : "OFF") + "\"",
//				"\"ENABLE_STEAM_SDK=" + std::string(data.enableSteamSdk ? "ON" : "OFF") + "\"",
//				"\"ENABLE_DISCORD_SDK=" + std::string(data.enableDiscordSdk ? "ON" : "OFF") + "\""
//			}
//		);
//
//		script.Run(true);
//	}
//
//	void GameModuleManager::Compile()
//	{
//		return;
//#ifdef _DEBUG
//		const bool isDebugBuild = true;
//#else
//		const bool isDebugBuild = false;
//#endif
//
//		static BatchScript script(
//			PathManager::GetProjectRoot() / "Tools",
//			"build-game-dll.bat",
//			{
//				std::string(isDebugBuild ? "Debug" : "Release")
//			}
//		);
//
//		script.Run(true);
//	}
//
//	void GameModuleManager::CompileAndLoad()
//	{
//		static std::string supportedExtensions[] = {
//			".hpp",
//			".cpp",
//			".h",
//			".inl"
//		};
//
//		bool wasChanged = false;
//		if (std::filesystem::exists(PathManager::GetGameDllBuildPath() / "Game.dll"))
//		{
//			auto lastWriteTime = std::filesystem::last_write_time(PathManager::GetGameDllBuildPath() / "Game.dll");
//
//			for (const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(PathManager::GetAssetsPath()))
//			{
//				for (const std::string& extension : supportedExtensions)
//				{
//					if (dir.path().extension() == extension)
//					{
//						auto fileWriteTime = std::filesystem::last_write_time(dir.path());
//
//						if (fileWriteTime > lastWriteTime)
//						{
//							wasChanged = true;
//						}
//					}
//				}
//			}
//		}
//		else
//		{
//			wasChanged = true;
//		}
//
//
//		if (wasChanged)
//		{
//			const auto& activeScene = SceneManager::GetActiveSceneAsset();
//			SceneManager::UnloadScene();
//
//			GameLoader::UnloadGameDLL();
//
//			std::filesystem::remove(PathManager::GetGameDllBuildPath() / "Game.dll");
//			std::filesystem::remove(PathManager::GetGameDllBuildPath() / "Game.pdb");
//
//			GenerateGameEditor();
//			Compile();
//
//			GameLoader::LoadGameDLL();
//
//			//SceneManager::LoadScene(activeScene);
//		}
//		else
//		{
//			GameLoader::UnloadGameDLL();
//			GameLoader::LoadGameDLL();
//		}
//	}
//}