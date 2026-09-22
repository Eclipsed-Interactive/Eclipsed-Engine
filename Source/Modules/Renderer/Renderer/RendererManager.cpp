#include "RendererManager.h"

#include <Windows.h>
#include <filesystem>

#include "IRenderer.h"
#include "Core/PathManager.h"

#include <assert.h>

namespace Eclipse::Graphics
{
	void* RendererManager::ActiveRendererDLL;
	RendererManager::DestroyRendererFn RendererManager::DestroyRenderer;

	IRenderer* RendererManager::ActiveRenderer = nullptr;

	IRenderer& RendererManager::LoadRenderer(RendererAPI api)
	{
		std::string rendererPath = "";
		switch (api)
		{
		case Eclipse::Graphics::RendererAPI::OpenGL: rendererPath = (PathManager::GetEngineRoot() / "Plugins" / "Eclipsed.OpenGL_Renderer.dll").generic_string();
			break;
		default:
			break;
		}

		if (!std::filesystem::exists(rendererPath))
		{
			char message[256];
			sprintf_s(
				message,
				"The renderer dll dont exist at path: %s",
				rendererPath
			);

			assert(false && message);
		}

		void* renderer = LoadLibraryA(rendererPath.c_str());
		if (!renderer)
		{
			const DWORD error = GetLastError();

			char message[256];
			sprintf_s(
				message,
				"LoadLibraryA failed. Error code: %lu",
				error
			);

			assert(false && message);
		}

		ActiveRendererDLL = renderer;

		void* createRendererFn = GetProcAddress((HMODULE)ActiveRendererDLL, "CreateRenderer");
		if (!createRendererFn)
		{
			FreeLibrary((HMODULE)ActiveRendererDLL);

			const DWORD error = GetLastError();

			char message[256];
			sprintf_s(
				message,
				"Cant retrieve exported methods: %lu",
				error
			);

			assert(false && message);
		}

		void* destroyRendererFn = GetProcAddress((HMODULE)ActiveRendererDLL, "DestroyRenderer");
		if (!destroyRendererFn)
		{
			FreeLibrary((HMODULE)ActiveRendererDLL);
			
			const DWORD error = GetLastError();

			char message[256];
			sprintf_s(
				message,
				"Cant retrieve exported methods: %lu",
				error
			);

			assert(false && message);
		}

		DestroyRenderer = reinterpret_cast<DestroyRendererFn>(destroyRendererFn);

		CreateRendererFn createRenderer = reinterpret_cast<CreateRendererFn>(createRendererFn);
		ActiveRenderer = createRenderer();

		return *ActiveRenderer;
	}

	IRenderer& RendererManager::GetRenderer()
	{
		return *ActiveRenderer;
	}
}
