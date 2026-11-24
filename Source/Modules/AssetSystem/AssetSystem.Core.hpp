#pragma once

#ifdef AssetSystem_EXPORTS
#define ASSETSYSTEM_API __declspec(dllexport)
#elif AssetSystem_IMPORTS
#define ASSETSYSTEM_API __declspec(dllimport)
#else
#define  ASSETSYSTEM_API
#endif