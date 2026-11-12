#include "ProfilerManager.h"

#include "ProfilerNode.h"
#include "CallTree.h"

namespace Eclipse::Profiler
{
	void ProfilerManager::BeginPhase(Phase phase)
	{
		ActivePhase = phase;
		ActiveTree = &working[(size_t)phase];
	}
	
	void ProfilerManager::EndPhase()
	{
		const size_t i = (size_t)ActivePhase;
		std::swap(working[i], completed[i]);

		ActiveTree = nullptr;
	}
	
	CallTree* ProfilerManager::Active()
	{
		return ActiveTree;
	}

	const CallTree& ProfilerManager::Completed(Phase phase)
	{
		return completed[(size_t)phase];
	}
}