#include "MainSingleton.h"

void Eclipse::MainSingleton::Destroy()
{
	for (auto& [type, singleton] : mySingletons)
	{
		if (!singleton.useDestructor) continue;
		singleton.deleter(singleton.instance);
	}
}
