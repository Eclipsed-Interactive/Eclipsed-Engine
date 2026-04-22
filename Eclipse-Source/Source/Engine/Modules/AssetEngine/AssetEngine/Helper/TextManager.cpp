#include "TextManager.h"

#include "FreeType/ft2build.h"
#include "CoreEngine/Debug/DebugLogger.h"

#include FT_FREETYPE_H  

namespace Eclipse
{
    TextManager::TextManager()
    {
        if (FT_Init_FreeType(&freeType))
        {
            LOG_ERROR("FreeType has FAILED to initialize");
        }
    }

    TextManager::~TextManager()
    {
    }
}