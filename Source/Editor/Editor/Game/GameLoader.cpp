//#include "GameLoader.h"
//
//#include "EclipsedEngine/ECS/ComponentManager.h"
//#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"
//#include <Windows.h>
//
//#include "EclipsedEngine/Core/PathManager.h"
//
//namespace Eclipse
//{
//	HMODULE GameLoader::gameDll;
//
//	void GameLoader::LoadGameDLL()
//	{
//		gameDll = LoadLibraryA((PathManager::GetGameDllBuildPath() / "Game.dll").generic_string().c_str());
//		if (!gameDll)
//		{
//			Reflection::ComponentRegistry::ClearRegisteredGameComponents();
//			return;
//		}
//
//		RegisterComponentsFunc registerFunc = (RegisterComponentsFunc)GetProcAddress(gameDll, "RegisterComponents");
//
//		ComponentRegistrySnapshot o = registerFunc();
//
//		Reflection::ComponentRegistry::ClearRegisteredGameComponents();
//
//		for (int i = 0; i < o.count; i++)
//		{
//			const auto comp = o.components[i];
//
//			Reflection::ComponentRegistry::RegisterComponent(
//				comp.name,
//				comp.rttiType,
//				[create = comp.createFunc, size = comp.size](unsigned gameObjId, unsigned compID) -> Component*
//				{
//					return ComponentManager::AddComponentWithID(gameObjId, compID, create, size);
//				},
//				[create = comp.createFunc, size = comp.size](unsigned gameObjId) -> Component*
//				{
//					return ComponentManager::AddComponent(gameObjId, create, size);
//				},
//				true
//			);
//		}
//	}
//
//	void GameLoader::UnloadGameDLL()
//	{
//		if (gameDll)
//		{
//			FreeLibrary(gameDll);
//		}
//	}
//}