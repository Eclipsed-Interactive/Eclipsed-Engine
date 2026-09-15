#pragma once

#include <vector>
#include <chrono>

class ProfilerNode
{
public:
	ProfilerNode(const char* aName, const char* aFile, int aLine);
	~ProfilerNode();

public:
	const char* GetName() const;
	const char* GetFilePath() const;

	int GetLine() const;

	const ProfilerNode* GetChild(int anIndex) const;
	ProfilerNode* GetChild(int anIndex);
	const std::vector<ProfilerNode*>& GetChildren() const;

	void AddChildren(ProfilerNode* aNode);

	void SetParentNode(ProfilerNode* aNode);
	const ProfilerNode* GetParentNode() const;
	ProfilerNode* GetParentNode();

	void SetStartTime();
	void SetEndTime();

	double GetDuration() const;
	void CalculateDuration();

	void AddMemoryAllocated(int size);
	void AddMemoryFreed(int size);

	int GetMemoryAllocated() const;

private:
	void SetCurrentTime(std::chrono::steady_clock::time_point& aTimePoint);

private:
	char* myName, * myFilePath;
	int myLine = -1;

	int memoryAllocated = 0;
	int memoryFreed = 0;

	std::chrono::steady_clock::time_point myStartTime, myEndTime;
	std::chrono::microseconds myDurationUs;

	std::vector<ProfilerNode*> myChildNodes;
	ProfilerNode* myParentNode = nullptr;
};