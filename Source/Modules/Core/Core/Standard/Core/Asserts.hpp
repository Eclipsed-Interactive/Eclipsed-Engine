#pragma once

#include "Core.Core.hpp"

namespace Eclipse
{
	CORE_API void AssertFailed(const char* expression, const char* file, int line);
}

// Will break the program if condition is false.
#define ASSERT(condition)		\
do {							\
	if(!(condition))			\
		Eclipse::AssertFailed(	\
			#condition, 		\
			__FILE__,			\
			__LINE__			\
		);						\
} while (false);
