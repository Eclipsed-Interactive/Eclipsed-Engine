#pragma once

#include "Renderer/RenderCommands/RenderCommand.h"

namespace Eclipse
{
	class Light2DCommand : public Graphics::RenderCommandBase
	{
	public:
		Light2DCommand(class PointLight2D* aLight) : myLight(aLight) {}
		~Light2DCommand() = default;

		void Execute() override;

	private:
		class PointLight2D* myLight;
	};
}