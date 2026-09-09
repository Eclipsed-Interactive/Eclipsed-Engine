#pragma once

#ifdef OpenGL_Renderer_EXPORTS
#define OPENGL_RENDERER_API __declspec(dllexport)
#elif OpenGL_Renderer_IMPORTS
#define OPENGL_RENDERER_API __declspec(dllimport)
#else
#define  OPENGL_RENDERER_API
#endif