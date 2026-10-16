#pragma once

#include "Core/Math/Vector/Vector2.h"
#include <vector>

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
        friend class OpenGL_FontAssetType;

        bool CharacterExists(char aChar)
        {
            return aChar < myCharTexture.size();
        }

        const Character& GetCharTexture(char aChar)
        {
            return myCharTexture[aChar];
        }

        float GetBiggestCharacterHeight()
        {
            return maxCharacterHeight;
        }

    private:
        std::vector<Character> myCharTexture;
        float maxCharacterHeight;
    };
}
