#include "PerformanceProfilerManager.h"

#include <fstream>
#include <iostream>

#include "ProfilerNode.h"

bool PerformanceProfilerManager::myCollectNextFrame;
ProfilerNode* PerformanceProfilerManager::myRootNode;
ProfilerNode* PerformanceProfilerManager::myCurrentNode;

void PerformanceProfilerManager::StartProfilingNode(ProfilerNode* aNode)
{
	if (myRootNode == nullptr)myRootNode = aNode;
	else aNode->SetParentNode(myCurrentNode);

	if (myCurrentNode != nullptr)
		myCurrentNode->AddChildren(aNode);

	myCurrentNode = aNode;

	myCurrentNode->SetStartTime();
}

void PerformanceProfilerManager::EndProfilingNode()
{
	myCurrentNode->SetEndTime();

	if (myCurrentNode->GetParentNode())
		myCurrentNode = myCurrentNode->GetParentNode();
}

void PerformanceProfilerManager::Print()
{
	auto buf = std::cout.rdbuf();
	std::ofstream file("F:/Projects/Eclipsed-Engine/output.txt");
	std::cout.rdbuf(file.rdbuf());

	PrintNode(myRootNode, 0);

	std::cout.rdbuf(buf);
}

void PerformanceProfilerManager::CollectNextFrame()
{
	myCollectNextFrame = true;
}

void PerformanceProfilerManager::Clear()
{
	if (myCollectNextFrame)
	{
		Print();
		myCollectNextFrame = false;
	}

	if (myRootNode) {
		delete myRootNode;
		myRootNode = nullptr;
	}

	myCurrentNode = nullptr;
}

void PerformanceProfilerManager::PrintNode(ProfilerNode* aNode, int in)
{
	for (int i = 0; i < in; i++) std::cout << "   ";

	aNode->CalculateDuration();
	std::cout << "| " << aNode->GetName() << "  |  " << aNode->GetDuration() << "us" << std::endl;
	for (auto node : aNode->GetChildren()) PrintNode(node, in + 1);
}

ScopedProfilerNodeRegistrator::ScopedProfilerNodeRegistrator(const char* aName, const char* aFile, int aLine)
{
	ProfilerNode* node = new ProfilerNode(aName, aFile, aLine);
	PerformanceProfilerManager::StartProfilingNode(node);
}

ScopedProfilerNodeRegistrator::~ScopedProfilerNodeRegistrator()
{
	PerformanceProfilerManager::EndProfilingNode();
}

ProfilerNodeRegistrator::ProfilerNodeRegistrator(const char* aName, const char* aFile, int aLine)
{
	ProfilerNode* node = new ProfilerNode(aName, aFile, aLine);
	PerformanceProfilerManager::StartProfilingNode(node);
}