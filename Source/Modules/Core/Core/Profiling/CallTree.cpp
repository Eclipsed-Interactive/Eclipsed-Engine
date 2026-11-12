#include "CallTree.h"

#include "ProfilerNode.h"

namespace Eclipse::Profiler
{
	void CallTree::Push(const char* aFunctionName, int aLine)
	{
		if (ActiveNode == nullptr)
		{
			Root = ActiveNode = new ProfilerNode(aFunctionName, aLine);
			return;
		}

		ActiveNode->children.emplace_back(new ProfilerNode(aFunctionName, aLine));

		ProfilerNode* parent = ActiveNode;
		ActiveNode = ActiveNode->children.back();
		ActiveNode->parent = parent;
	}

	void CallTree::Pop(std::chrono::microseconds duration)
	{
		ActiveNode->duration = duration;
		ActiveNode = ActiveNode->parent;
	}
}