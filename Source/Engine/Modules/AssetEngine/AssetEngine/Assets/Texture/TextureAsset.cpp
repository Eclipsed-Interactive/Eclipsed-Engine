#include "TextureAsset.h"

namespace Eclipse::Assets
{
    Math::Vector2f Texture::GetDimDivOne() const
    {
        return dataPtr->dimDivOne;
    }

    Math::Vector2f Texture::GetSizeNormalized() const
    {
        return Math::Vector2f();
    }

    int Texture::GetWidth() const
    {
        return dataPtr->width;
    }

    int Texture::GetHeight() const
    {
        return dataPtr->height;
    }

    int Texture::GetChannels() const
    {
        return dataPtr->channels;
    }

    const std::vector<Math::RectSizePos> Texture::GetSpriteRects() const
    {
        return dataPtr->spriteRects;
    }

    void Texture::Bind()
    {

    }
}