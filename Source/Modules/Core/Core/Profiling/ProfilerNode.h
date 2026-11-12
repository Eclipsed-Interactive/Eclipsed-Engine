#pragma once

#include <chrono>

namespace Eclipse::Profiler
{
	struct ProfilerNode
	{
		ProfilerNode(const char* aFunctionName, int aLine);
		
		ProfilerNode* parent = nullptr;
		std::vector<ProfilerNode*> children;

		std::string functionName = "";
		int line = -1;

		std::chrono::microseconds duration = {};
	};
}