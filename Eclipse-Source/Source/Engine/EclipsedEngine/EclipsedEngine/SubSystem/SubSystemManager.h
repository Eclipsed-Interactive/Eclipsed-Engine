#pragma once

#include "CoreEngine/Macros/defines.h"
#include "SubSystem.h"
#include <vector>

namespace Eclipse
{
	class ECLIPSED_API SubSystemManager {
	public:
		static void RegisterSystem(void* aSubSystem);

	public:
		static void InitSystems();
		static void ExecuteSystems();

		static std::vector<SubSystem*> myRegisteredSystems;
	};
}