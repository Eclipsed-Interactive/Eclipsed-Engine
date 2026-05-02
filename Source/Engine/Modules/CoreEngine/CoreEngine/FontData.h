#pragma once

#include "CoreEngine/Math/Vector/Vector2.h"
#include <unordered_map>

namespace Eclipse::Assets
{
    struct Character
    {
        unsigned textureID;
        Math::Vector2i size;
        Math::Vector2i bearing;
        int advance;
    };
    
    struct InternalFont
    {
        std::unordered_map<char, Character> myCharTexture;
        float maxCharHeight;
    };
}
