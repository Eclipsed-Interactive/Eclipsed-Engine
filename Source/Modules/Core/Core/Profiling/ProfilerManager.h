#pragma once

#include <array>

#include "Core.Core.hpp"

#include "CallTree.h"

namespace Eclipse::Profiler
{
	enum class Phase : int
	{
		Init,
		Update,

		Count
	};

	//CORE_API constexpr std::string_view ToString(ProfilerTreeType aType)
	//{
	//	switch (aType)
	//	{
	//	case ProfilerTreeType::Init:   return "Init";
	//	case ProfilerTreeType::Update: return "Update";
	//	default:               return "Unknown";
	//	}
	//}

	class CORE_API ProfilerManager final
	{
	public:
		static void BeginPhase(Phase);
		static void EndPhase();
		static CallTree* Active();
		static const CallTree& Completed(Phase);

	private:
		static inline std::array<CallTree, (size_t)Phase::Count> working;
		static inline std::array<CallTree, (size_t)Phase::Count> completed;
		static inline CallTree* ActiveTree;
		static inline Phase ActivePhase;
	};
}