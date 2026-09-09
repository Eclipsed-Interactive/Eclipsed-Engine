#pragma once

#include "Input/AbstractInput.h"
#include "IGraphicsBuffer.h"
#include "UniformVariableManager.h"
#include "IDrawer.h"
#include "IGraphicsDevice.h"

#include "Renderer.Core.hpp"

class ErrorCode {};

namespace Eclipse::Graphics
{
	class RENDERER_API IRenderer
	{
	public:
		virtual ErrorCode Init() { return ErrorCode{}; };

		virtual void BeginFrame() {};
		virtual void EndFrame() {};

		virtual bool ShouldWindowClose() { return true; };

		virtual void Render() {};

		virtual void ImGui_NewFrame() {};
		virtual void ImGui_Init(void* imguiCtx) {};
		virtual void ImGui_Render() {};

		virtual Input::AbstractInput* CreateInput() = 0;

		IGraphicsBuffer* GetGraphicsBuffer();
		UniformVariableManager* GetUniformVariableManager();
		IDrawer* GetDrawer();
		IGraphicsDevice* GetDevice();

	protected:
		IGraphicsBuffer* graphicsBuffer;
		UniformVariableManager* uniformVariableManager;
		IDrawer* drawer;
		IGraphicsDevice* device;
	};
}