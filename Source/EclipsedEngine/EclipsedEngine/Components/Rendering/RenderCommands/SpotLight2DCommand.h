#pragma once

#include "Renderer/RenderCommands/RenderCommand.h"

namespace Eclipse
{
	class SpotLight2DCommand : public Graphics::RenderCommandBase
	{
	public:
		SpotLight2DCommand(class SpotLight2D* aLight) : myLight(aLight) {}
		~SpotLight2DCommand() = default;

		void Execute() override;

	private:
		class SpotLight2D* myLight;
	};
}