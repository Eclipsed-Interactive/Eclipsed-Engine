//#pragma once
//
//#include <wtypes.h>
//#include "EclipsedEngine/ECS/Component.h"
//
//#include "EclipsedEngine.Core.hpp"
//
//namespace Eclipse
//{
//	struct ComponentEntry final
//	{
//		const char* name;
//		const char* rttiType;
//		Component* (__cdecl* createFunc)(unsigned char* address);
//		size_t size;
//	};
//
//	struct ComponentRegistrySnapshot final
//	{
//		ComponentEntry* components = nullptr;
//		int count = 0;
//	};
//
//
//
//	typedef ComponentRegistrySnapshot(*RegisterComponentsFunc)();
//
//	class ECL_API GameLoader
//	{
//	public:
//		static void LoadGameDLL();
//		static void UnloadGameDLL();
//
//	private:
//		static HMODULE gameDll;
//	};
//}