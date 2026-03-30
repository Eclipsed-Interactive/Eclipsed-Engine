#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <vector>

namespace Eclipse::Debug
{
	struct ProfilerNode
	{
	public:
		ProfilerNode(ProfilerNode* aParentNode);

		ProfilerNode* GetNode(int aIdx);
		int GetNodeCount() const;

		void AddChild(ProfilerNode* aNode);

		const std::chrono::high_resolution_clock::time_point& GetStartTime() const;
		const std::chrono::high_resolution_clock::time_point& GetEndTime() const;

		std::string_view GetName() const;
		void SetName(const std::string_view& aName);

		void SetStartTime();
		void SetEndTime();

	private:
		void SetCurrentTime(std::chrono::high_resolution_clock::time_point& aTimePoint);

	private:
		std::vector<ProfilerNode*> myChildren;

		std::string myName;
		std::chrono::high_resolution_clock::time_point myStartTime;
		std::chrono::high_resolution_clock::time_point myEndTime;

		ProfilerNode* myParentNode = nullptr;
	};
}