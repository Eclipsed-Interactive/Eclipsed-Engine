#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <mutex>

// ----------- Files
#include "CoreEngine/Files/DirectoryTree.h"
#include "CoreEngine/Files/File.h"
#include "CoreEngine/Files/FileInfo.h"
#include "CoreEngine/Files/FileUtilities.h"
#include "CoreEngine/Files/FileWatcher.h"

// ----------- Input

// ----------- Macros
#include "CoreEngine/Macros/defines.h"

// ----------- Math
#include "CoreEngine/Math/Color.h"
#include "CoreEngine/Math/CommonMath.h"
#include "CoreEngine/Math/Math.h"
#include "CoreEngine/Math/Random.h"
#include "CoreEngine/Math/Rect.h"
#include "CoreEngine/Math/RectSizePos.h"
#include "CoreEngine/Math/Matrix/Matrix2x2.h"
#include "CoreEngine/Math/Matrix/Matrix3x3.h"
#include "CoreEngine/Math/Vector/Vector2.h"
#include "CoreEngine/Math/Vector/Vector3.h"
#include "CoreEngine/Math/Vector/Vector4.h"

// ----------- Platform Integration
#include "CoreEngine/PlatformIntegration/DiscordIntegration.h"
#include "CoreEngine/PlatformIntegration/IntegrationManager.h"

// ----------- Settings
#include "CoreEngine/Settings/EngineSettings.h"
#include "CoreEngine/Settings/GraphicsSettings.h"

// ----------- Containers
#include "CoreEngine/Containers/RingBuffer.h"

// ----------- Other
#include "CoreEngine/BitMask.hpp"
#include "CoreEngine/ClipBoard.h"
#include "CoreEngine/Engine.h"
#include "CoreEngine/ErrorCodes.h"
#include "CoreEngine/MainSingleton.h"
#include "CoreEngine/PathManager.h"
#include "CoreEngine/SerializedEnum.hpp"
#include "CoreEngine/Timer.h"
#include "CoreEngine/TypeChecks.h"