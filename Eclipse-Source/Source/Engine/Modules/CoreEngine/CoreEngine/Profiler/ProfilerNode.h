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

	long long GetDuration() const;
	void CalculateDuration();

private:
	void SetCurrentTime(std::chrono::steady_clock::time_point& aTimePoint);

private:
	char* myName,* myFilePath;
	int myLine = -1;

	std::chrono::steady_clock::time_point myStartTime, myEndTime;
	long long myDurationMs = -1;

	std::vector<ProfilerNode*> myChildNodes;
	ProfilerNode* myParentNode = nullptr;
};