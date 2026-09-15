#pragma once

#ifdef Core_EXPORTS
#define CORE_API __declspec(dllexport)
#elif Core_IMPORTS
#define CORE_API __declspec(dllimport)
#else
#define  CORE_API
#endif