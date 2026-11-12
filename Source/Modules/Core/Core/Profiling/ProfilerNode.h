#pragma once

#include <chrono>

namespace Eclipse::Profiler
{
	struct ProfilerNode
	{
		ProfilerNode(const char* aFunctionName);
		
		ProfilerNode* parent = nullptr;
		std::vector<ProfilerNode*> children;

		std::string functionName = "";

		std::chrono::microseconds duration = {};
	};
}