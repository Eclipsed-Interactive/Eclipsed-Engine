#include "PluginManager.h"

#include "Core/BatchScript.h"
#include "Core/PathManager.h"

#include <fstream>
#include <iostream>
#include <Windows.h>

namespace Eclipse
{
	void PluginManager::Compile(const char* source)
	{
		Plugin* plug = new Plugin(source);
		if (!plug->Succeded())
		{
			delete plug;
			return;
		}

		loadedplugins[plug->GetName()] = plug;
		plug->GetInitFunc()();
	}
}