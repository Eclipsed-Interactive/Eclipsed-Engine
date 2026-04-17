#pragma once

#include <fstream>

#include "cereal/access.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/cereal.hpp"

#include "CoreEngine/PathManager.h"
#include "CoreEngine/EventSystem/EventSystem.h"

#define BASE_SETTINGS(_SETTING, _NAME)																		\
_SETTING() = default;																						\
~_SETTING() = default; 																						\
struct TEMP { 																								\
	TEMP() { 																								\
		Eclipse::EventSystem::Subscribe("Engine-Load", _SETTING::Load);										\
		Eclipse::EventSystem::Subscribe("Engine-Shutdown", _SETTING::Save);									\
	}																										\
};																											\
static inline TEMP temp = {};																				\
static inline _SETTING::Data& GetData() {																	\
	static _SETTING::Data data{};																			\
	return data;																							\
}																											\
static constexpr const char* Name = _NAME;																	\
static constexpr const char* SettingsName() { return _SETTING::Name; }	

#define SETTINGS_DATA struct Data

#define MAKE_NVP(Var) CEREAL_NVP(Var)

#define SERIALIZE(...)                                  \
    template <class Archive>                            \
    void serialize(Archive& ar)                         \
    {                                                   \
        ar(__VA_ARGS__);                                \
    }

#define GET_SET(_TYPE)																		\
	static const decltype(Data::_TYPE)& Get##_TYPE() {return GetData()._TYPE;}				\
	static void Set##_TYPE(const decltype(Data::_TYPE)& data) {GetData()._TYPE = data;}

namespace Eclipse::Settings 
{
	template<typename Derived>
	struct BaseSettings
	{
		static inline void Save()
		{
			std::ofstream out(PathManager::GetSettingsPath() / Derived::Name);

			cereal::JSONOutputArchive archive(out);
			archive(cereal::make_nvp(Derived::Name, Derived::GetData()));
		}

		static inline void Load()
		{
			std::filesystem::path path = PathManager::GetSettingsPath() / Derived::Name;
			std::ifstream in(path);
			if (!in.is_open()) {
				Save();
				return;
			}

			cereal::JSONInputArchive archive(in);
			archive(Derived::GetData());
		}
	};
}