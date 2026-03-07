#include "SubSystemManager.h"

namespace Eclipse 
{
	std::vector<SubSystem*> SubSystemManager::myRegisteredSystems;

	void SubSystemManager::RegisterSystem(void* aSubSystem)
	{
		if (aSubSystem == nullptr) return;
		myRegisteredSystems.push_back((SubSystem*)aSubSystem);
	}

	void SubSystemManager::InitSystems()
	{
		for (SubSystem* subSys : myRegisteredSystems) {
			subSys->Init();
		}
	}

	void SubSystemManager::ExecuteSystems()
	{
		for (SubSystem* subSys : myRegisteredSystems) {
			subSys->Execute();
		}
	}
}