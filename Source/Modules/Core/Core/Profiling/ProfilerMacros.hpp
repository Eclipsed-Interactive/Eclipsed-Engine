#pragma once

#include "ProfileScope.h"

#define ECL_CONCAT_IMPL(a, b) a##b
#define ECL_CONCAT(a, b) ECL_CONCAT_IMPL(a, b)

#define PROFILE_SCOPED ::Eclipse::Profiler::Scope ECL_CONCAT(ProfileScope, __LINE__)(__FUNCTION__, __LINE__);