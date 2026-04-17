#pragma once

#include "CoreEngine/Input/Input.h"

namespace Eclipse
{
	struct ComponentEntry final
	{
		const char* name;
		Eclipse::Component* (__cdecl* createFunc)(unsigned char* address);
		size_t size;
	};

	struct ComponentRegistrySnapshot final
	{
		ComponentEntry* components = nullptr;
		int count = 0;
	};



	typedef ComponentRegistrySnapshot(*RegisterComponentsFunc)();

	HMODULE gameDll;

	class GameLoader
	{
	public:
		static void LoadGameDLL();
		static void UnloadGameDLL();
	};
}