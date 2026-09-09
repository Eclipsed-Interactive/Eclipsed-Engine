#include "MainSingleton.h"

namespace Eclipse
{
    std::unordered_map<std::string, SingletonEntry> MainSingleton::mySingletons;
}