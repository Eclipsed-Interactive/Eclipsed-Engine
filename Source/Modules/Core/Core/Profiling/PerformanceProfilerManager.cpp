#include "Profiling/PerformanceProfilerManager.h"

#include <fstream>
#include <iostream>

#include "Profiling/ProfilerNode.h"

bool PerformanceProfilerManager::myCollectNextFrame;
ProfilerNode* PerformanceProfilerManager::myRootNode;
ProfilerNode* PerformanceProfilerManager::myCurrentNode;

#include <cstdlib>
#include <new>
#include <cstdio>

void* operator new(std::size_t size) {
	void* ptr = std::malloc(size);

	ProfilerNode* node = PerformanceProfilerManager::GetCurrentNode();
	if (node != nullptr)
	{
		node->AddMemoryAllocated((int)size);
	}

	if (!ptr) throw std::bad_alloc();

	return ptr;
}

void operator delete(void* ptr) noexcept {
	std::free(ptr);
}


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
	std::ofstream file("output.txt");
	std::cout.rdbuf(file.rdbuf());

	PrintNode(myRootNode, 0);

	std::cout.rdbuf(buf);
}

void PerformanceProfilerManager::CollectNextFrame()
{
	myCollectNextFrame = true;
}

ProfilerNode* PerformanceProfilerManager::GetCurrentNode()
{
	return myCurrentNode;
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

void PerformanceProfilerManager::PrintNode(ProfilerNode* aNode, int depth)
{
	if (aNode == nullptr) return;

	aNode->CalculateDuration();

	std::string indent;
	indent.reserve(depth * 3);

	for (int i = 0; i < depth - 1; i++)
		indent += "│";
	//"├─ "

	std::cout
		<< indent
		<< (depth > 0 ? "└─ " : "")
		<< std::left << std::setw(45 - depth * 2) << aNode->GetName()
		<< std::right << std::setw(8) << std::fixed << std::setprecision(3)
		<< aNode->GetDuration() << " ms"
		<< "   alloc: "
		<< aNode->GetMemoryAllocated()
		<< "\n";

	for (auto node : aNode->GetChildren())
		PrintNode(node, depth + 1);
}

ScopedProfilerNodeRegistrator_CORE::ScopedProfilerNodeRegistrator_CORE(const char* aName, const char* aFile, int aLine)
{
	ProfilerNode* node = new ProfilerNode(aName, aFile, aLine);
	PerformanceProfilerManager::StartProfilingNode(node);
}

ScopedProfilerNodeRegistrator_CORE::~ScopedProfilerNodeRegistrator_CORE()
{
	PerformanceProfilerManager::EndProfilingNode();
}

ProfilerNodeRegistrator_CORE::ProfilerNodeRegistrator_CORE(const char* aName, const char* aFile, int aLine)
{
	ProfilerNode* node = new ProfilerNode(aName, aFile, aLine);
	PerformanceProfilerManager::StartProfilingNode(node);
}