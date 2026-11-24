#pragma once

// This is typedefed as "FT_Library" in freetype
class FT_LibraryRec_;

#include "FreeType/ft2build.h"

#include FT_FREETYPE_H  

namespace Eclipse
{
    class TextManager
    {
    public:
        inline TextManager()
        {
            if (FT_Init_FreeType(&freeType))
            {
                //CORE_LOG_ERROR("FreeType has FAILED to initialize");
            }
        }
        ~TextManager() = default;

        FT_LibraryRec_* freeType;
    };
}