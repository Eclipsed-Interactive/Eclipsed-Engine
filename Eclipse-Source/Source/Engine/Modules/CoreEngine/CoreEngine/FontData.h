#pragma once

namespace Eclipse
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
