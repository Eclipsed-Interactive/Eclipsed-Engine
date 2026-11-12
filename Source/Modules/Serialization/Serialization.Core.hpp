#pragma once

#ifdef Serialization_EXPORTS
#define SERIALIZATION_API __declspec(dllexport)
#elif Serialization_IMPORTS
#define SERIALIZATION_API __declspec(dllimport)
#else
#define  SERIALIZATION_API
#endif