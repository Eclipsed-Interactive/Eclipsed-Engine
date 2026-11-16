#pragma once

#include "Renderer/RenderCommands/RenderCommand.h"

namespace Eclipse
{
	class PointLight2DCommand : public Graphics::RenderCommandBase
	{
	public:
		PointLight2DCommand(class PointLight2D* aLight) : myLight(aLight) {}
		~PointLight2DCommand() = default;

		void Execute() override;

	private:
		class PointLight2D* myLight;
	};
}