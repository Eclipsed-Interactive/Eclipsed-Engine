#include "ProfileScope.h"

#include "ProfilerManager.h"

namespace Eclipse::Profiler
{
	Scope::Scope(const char* aFunctionName, int aLine)
		: myTree(ProfilerManager::Active())
	{
		myStartTime = std::chrono::steady_clock::now();
		ProfilerManager::Active()->Push(aFunctionName, aLine);
	}

	Scope::~Scope()
	{
		if (!myTree) return;
		ProfilerManager::Active()->Pop(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - myStartTime));
	}
}