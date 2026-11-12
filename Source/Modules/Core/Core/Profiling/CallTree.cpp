#include "CallTree.h"

#include "ProfilerNode.h"

namespace Eclipse::Profiler
{
	void CallTree::Push(const char* aFunctionName)
	{
		if (ActiveNode == nullptr)
		{
			Root = ActiveNode = new ProfilerNode(aFunctionName);
			return;
		}

		ActiveNode->children.emplace_back(new ProfilerNode(aFunctionName));

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