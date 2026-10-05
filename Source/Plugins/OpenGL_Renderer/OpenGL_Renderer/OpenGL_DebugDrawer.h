#pragma once

#include "Renderer/IDebugDrawer.h"

namespace Eclipse::Graphics::OpenGL
{
	class OpenGL_DebugDrawer : public IDebugDrawer
	{
	public:
		void Init() override;
		void Render() override;

        unsigned myVTXbuffer;
        unsigned myIDXbuffer;
        unsigned myLineBuffer;

        unsigned pixelShaderID;
        unsigned vtxShaderID;
        unsigned programID;
	};
}