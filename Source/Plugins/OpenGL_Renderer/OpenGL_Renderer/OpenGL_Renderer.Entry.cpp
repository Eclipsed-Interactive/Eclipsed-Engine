#include "OpenGL_Renderer.Entry.h"

#include "OpenGL_Renderer.h"

using namespace Eclipse::Graphics;
using namespace Eclipse::Graphics::OpenGL;

OPENGL_RENDERER_API IRenderer* CreateRenderer()
{
	return new OpenGL_Renderer;
}

OPENGL_RENDERER_API void DestroyRenderer(IRenderer* renderer)
{
	// Needs to be implemented.
}
