#include "Asserts.hpp"

namespace Eclipse
{
	CORE_API void AssertFailed(const char* expression, const char* file, int line)
	{
#ifdef _MSC_VER
		__debugbreak();
#endif
	}
}