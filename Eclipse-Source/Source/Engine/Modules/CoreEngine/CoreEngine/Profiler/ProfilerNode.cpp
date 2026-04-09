#include "ProfilerNode.h"

#include <assert.h>
#include <string.h>

#include "PerformanceProfilerManager.h"

ProfilerNode::ProfilerNode(const char* aName, const char* aFile, int aLine)
	: myLine(aLine)
{
	myName = new char[strlen(aName) + 1];
	strcpy_s(myName, strlen(aName) + 1, aName);

	myFilePath = new char[strlen(aFile) + 1];
	strcpy_s(myFilePath, strlen(aFile) + 1, aFile);
}

ProfilerNode::~ProfilerNode()
{
	delete[] myName;
	delete[] myFilePath;

	for (ProfilerNode* node : myChildNodes)
	{
		delete node;
	}

	myChildNodes.clear();
}

const char* ProfilerNode::GetName() const
{
	return myName;
}

const char* ProfilerNode::GetFilePath() const
{
	return myFilePath;
}

int ProfilerNode::GetLine() const
{
	return myLine;
}

const ProfilerNode* ProfilerNode::GetChild(int anIndex) const
{
	assert(myChildNodes.size() < anIndex);
	return myChildNodes[anIndex];
}

ProfilerNode* ProfilerNode::GetChild(int anIndex)
{
	assert(myChildNodes.size() < anIndex);
	return myChildNodes[anIndex];
}

const std::vector<ProfilerNode*>& ProfilerNode::GetChildren() const
{
	return myChildNodes;
}

void ProfilerNode::AddChildren(ProfilerNode* aNode)
{
	myChildNodes.push_back(aNode);
}

void ProfilerNode::SetParentNode(ProfilerNode* aNode)
{
	myParentNode = aNode;
}

const ProfilerNode* ProfilerNode::GetParentNode() const
{
	return myParentNode;
}

ProfilerNode* ProfilerNode::GetParentNode()
{
	return myParentNode;
}

void ProfilerNode::SetStartTime()
{
	SetCurrentTime(myStartTime);
}

void ProfilerNode::SetEndTime()
{
	SetCurrentTime(myEndTime);
	CalculateDuration();
}

long long ProfilerNode::GetDuration() const
{
	return myDurationMs;
}

void ProfilerNode::CalculateDuration()
{
	myDurationMs = std::chrono::duration_cast<std::chrono::microseconds>(myEndTime - myStartTime).count();
}

void ProfilerNode::SetCurrentTime(std::chrono::steady_clock::time_point& aTimePoint)
{
	aTimePoint = std::chrono::steady_clock::now();
}