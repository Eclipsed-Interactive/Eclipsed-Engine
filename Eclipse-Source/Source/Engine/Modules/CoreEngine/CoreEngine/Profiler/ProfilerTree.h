#pragma once

#include <iostream>
#include<chrono>

#include <vector>

#include "ProfilerNodes.h"

namespace Eclipse::Debug
{
	class ProfilerTree
	{
		void AddNode();

		ProfilerNode* myRootNode;
	};
}