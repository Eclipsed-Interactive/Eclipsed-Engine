#pragma once

#include "Core.Core.hpp"

class ProfilerNode;

struct CORE_API ScopedProfilerNodeRegistrator_CORE
{
	ScopedProfilerNodeRegistrator_CORE(const char* aName, const char* aFile, int aLine);
	~ScopedProfilerNodeRegistrator_CORE();
};

struct CORE_API ProfilerNodeRegistrator_CORE
{
	ProfilerNodeRegistrator_CORE(const char* aName, const char* aFile, int aLine);
};

#define CONCAT(a, b) a##b
#define CONCAT_EXPAND(a, b) CONCAT(a, b)
#define GET_NAME(name) CONCAT_EXPAND(name, __LINE__) 

#define CORE_PROFILE_SCOPED ScopedProfilerNodeRegistrator_CORE GET_NAME(registrator_)(__FUNCTION__, __FILE__, __LINE__);

#define PROFILE_BEGIN(name) ProfilerNodeRegistrator GET_NAME(registrator_)(name, __FILE__, __LINE__);
#define PROFILE_END PerformanceProfilerManager::EndProfilingNode();

class CORE_API PerformanceProfilerManager
{
public:
	static void StartProfilingNode(ProfilerNode* aNode);
	static void EndProfilingNode();

	static void Clear();

	static void Print();

	static void CollectNextFrame();

	static ProfilerNode* GetCurrentNode();

private:
	static void PrintNode(ProfilerNode* aNode, int in);

private:
	static ProfilerNode* myRootNode;
	static ProfilerNode* myCurrentNode;

	static bool myCollectNextFrame;
};