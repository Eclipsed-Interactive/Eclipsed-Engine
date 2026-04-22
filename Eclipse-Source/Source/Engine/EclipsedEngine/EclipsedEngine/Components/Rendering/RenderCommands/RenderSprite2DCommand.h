#pragma once

#include "GraphicsEngine/RenderCommands/RenderCommand.h"



// HOW THE FUCK IS THIS A COMPONENT (Asked in a nice way) ITS FINE... (HE SAID CALMLY!)




namespace Eclipse
{
    class RenderSprite2DCommand : public RenderCommandBase
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