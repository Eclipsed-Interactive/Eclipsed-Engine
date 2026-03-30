#include "ProfilerNodes.h"

namespace Eclipse::Debug
{
	std::string_view ProfilerNode::GetName() const
	{
		return myName;
	}

	void ProfilerNode::SetName(const std::string_view& aName)
	{
		myName = aName;
	}

	ProfilerNode::ProfilerNode(ProfilerNode* aParentNode)
	{

	}

	ProfilerNode* ProfilerNode::GetNode(int aIdx)
	{
		if (myChildren.size() < aIdx) return myChildren[aIdx];
		return nullptr;
	}

	int ProfilerNode::GetNodeCount() const
	{
		return (int)myChildren.size();
	}

	void ProfilerNode::AddChild(ProfilerNode* aNode)
	{
		myChildren.push_back(aNode);
	}

	const std::chrono::high_resolution_clock::time_point& ProfilerNode::GetStartTime() const
	{
		return myStartTime;
	}

	const std::chrono::high_resolution_clock::time_point& ProfilerNode::GetEndTime() const
	{
		return myEndTime;
	}

	void ProfilerNode::SetStartTime()
	{
		SetCurrentTime(myStartTime);
	}
	
	void ProfilerNode::SetEndTime()
	{
		SetCurrentTime(myEndTime);
	}
	
	void ProfilerNode::SetCurrentTime(std::chrono::high_resolution_clock::time_point& aTimePoint)
	{
		aTimePoint = std::chrono::high_resolution_clock::now();
	}
}
