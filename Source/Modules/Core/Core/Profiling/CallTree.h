#pragma once

#include <chrono>

#include "Core.Core.hpp"

namespace Eclipse::Profiler
{
	class CallTree  final
	{
	public:
		void Push(const char* aFunctionName, int aLine);
		void Pop(std::chrono::microseconds duration);

		class ProfilerNode* GetRoot() { return Root; }
		const class ProfilerNode* GetRoot() const { return Root; }

	private:
		class ProfilerNode* Root = nullptr;
		class ProfilerNode* ActiveNode = nullptr;
	};
}