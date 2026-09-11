#pragma once

#ifdef Reflection_EXPORTS
#define REFLECTION_API __declspec(dllexport)
#elif Reflection_IMPORTS
#define REFLECTION_API __declspec(dllimport)
#else
#define  REFLECTION_API
#endif