#pragma once

#include "OpenGL_Renderer.Core.hpp"
#include "Renderer/IRenderer.h"

extern "C"
{
    OPENGL_RENDERER_API Eclipse::Graphics::IRenderer* CreateRenderer();
    OPENGL_RENDERER_API void DestroyRenderer(Eclipse::Graphics::IRenderer* renderer);
}