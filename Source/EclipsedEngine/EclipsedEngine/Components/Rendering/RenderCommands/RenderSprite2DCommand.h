#pragma once

#include "Renderer/RenderCommands/RenderCommand.h"

namespace Eclipse
{
    class SpriteRenderer2D;
    class RenderSprite2DCommand : public Graphics::RenderCommandBase
    {
    public:
        RenderSprite2DCommand(SpriteRenderer2D* aSpriteRend) : mySpriteRenderer(aSpriteRend)
        {}
        ~RenderSprite2DCommand() = default;

        void Execute() override;

    private:
        class SpriteRenderer2D* mySpriteRenderer;
    };
}