#pragma once

#include "Renderer.Core.hpp"
#include <unordered_map>
#include "IRenderer.h"

namespace Eclipse::Graphics
{
	enum class RendererAPI
	{
		OpenGL
	};

	class RENDERER_API RendererManager final
	{
	public:
		static IRenderer& LoadRenderer(RendererAPI api);

	public:
		static IRenderer& GetRenderer();

	private:
		using CreateRendererFn = IRenderer * (*)();
		using DestroyRendererFn = void (*)(IRenderer*);

		static void* ActiveRendererDLL;
		static DestroyRendererFn DestroyRenderer;

		static IRenderer* ActiveRenderer;
	};
}