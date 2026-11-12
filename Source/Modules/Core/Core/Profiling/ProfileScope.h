#pragma once

#include <chrono>
#include "Core.Core.hpp"

namespace Eclipse::Profiler
{
	class CORE_API Scope final
	{
	public:
		Scope(const char* aFunctionName, int aLine);
		~Scope();

	private:
		class CallTree* myTree = nullptr;
		std::chrono::steady_clock::time_point myStartTime;
	};
}