#include "GameWindow.h"

#include "ImGui/imgui.h"

#include "CoreEngine/Math/Math.h"

#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"
#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "EclipsedEngine/Components/Rendering/Camera.h"

#include "CoreEngine/Input/Input.h"

#include "OpenGL\glad\glad.h"

#include <array>
#include <sstream>

#include "CoreEngine/MainSingleton.h"
#include "CoreEngine/Settings/GraphicsSettings.h"

#include "EclipsedEngine/Input/Input.h"


namespace Eclipse::Editor
{
	struct TResolution
	{
		std::string name;
		Math::Vector2f res;
	};

	std::vector<TResolution> resolutionChooser{ {"Free Aspect", {0.f, 0.f}}, {"16:9", {16.f, 9.f}}, {"16:10", {16.f, 10.f}}, {"4:3", {4.f, 3.f}}, {"21:9", {21.f, 9.f}} };

	void GameWindow::Update()
	{
		if (ImGui::BeginMenuBar())
		{
			std::stringstream ss;
			ss << "Resolution: " << resolutionChooser[currentResIndex].name;

			if (ImGui::Button(ss.str().c_str()))
				ImGui::OpenPopup("Res");

			if (ImGui::BeginPopup("Res"))
			{
				for (int i = 0; i < resolutionChooser.size(); i++)
				{
					if (ImGui::Selectable(resolutionChooser[i].name.c_str()))
					{
						currentResIndex = i;
						myCurrentWindowMode = static_cast<WindowMode>(static_cast<bool>(i));

						Math::Vector2f resolution = resolutionChooser[i].res;
						myWindowResAspect = { resolution.x / resolution.y, resolution.y / resolution.x };
					}

				}

				ImGui::EndPopup();
			}

			ImGui::Checkbox("Draw Gizmos", &myDrawGameGizmos);

			//DrawGizmoButtons(DrawGizmo);

			ImGui::EndMenuBar();
		}

		ImVec2 windowSize = ImGui::GetWindowSize();
		if (!Camera::main)
		{
			ImVec2 size = ImGui::CalcTextSize("No Camera Rendering");
			ImGui::SetCursorPos({ windowSize.x * 0.5f - size.x * 0.5f, windowSize.y * 0.5f - size.y * 0.5f + 43 * 0.5f });
			ImGui::Text("No Camera Rendering");
			return;
		}



		// These clear colors are not working like they should and get mixed up so the first is the empty background and second is the actual
		GraphicsEngine::BindFrameBuffer(myGameFrameBuffer);
		GraphicsEngine::ClearCurrentSceneBuffer();

		if (myCurrentWindowMode != FreeAspect)
			UpdateSpecifiedRes();
		else
			UpdateFreeAspect();

		GraphicsEngine::BindFrameBuffer(0);
	}

	void GameWindow::UpdateFreeAspect()
	{
		ImVec2 CursorPos = ImGui::GetCursorPos();
		CursorPos.x -= 8;
		CursorPos.y -= 7;

		ImVec2 windowSize = ImGui::GetWindowSize();
		glViewport(0, 0, windowSize.x, windowSize.y + 44);

		float aspectRatio = windowSize.y / windowSize.x;
		GraphicsEngine::UpdateGlobalUniform(UniformType::Float, "resolutionRatio", &aspectRatio);

		CommandListManager::GetSpriteCommandList().Execute();
		CommandListManager::GetUICommandList().Execute();
		if (myDrawGameGizmos) CommandListManager::GetDebugDrawCommandList().Execute();

		if (windowSize.x != myLastWindowResolution.x || windowSize.y != myLastWindowResolution.y)
		{
			glBindTexture(GL_TEXTURE_2D, myGameTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);

			// Also update the framebuffer attachment
			glBindFramebuffer(GL_FRAMEBUFFER, myGameFrameBuffer);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myGameTexture, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		myLastWindowResolution = { static_cast<int>(windowSize.x), static_cast<int>(windowSize.y) };

		Settings::GraphicsSettings::SetResolution(Math::Vector2i((int)windowSize.x, (int)windowSize.y));

		ImGui::SetCursorPos(CursorPos);

		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
		float mousePosX = mousePos.x - cursorScreenPos.x;
		float mousePosY = windowSize.y - (mousePos.y - cursorScreenPos.y);
		Input::SetGamePosition({ static_cast<int>(mousePosX), static_cast<int>(mousePosY) });

		ImGui::Image(myGameTexture, ImVec2(windowSize.x, windowSize.y - CursorPos.y), ImVec2(0, 1), ImVec2(0.99, 0));

		//MainSingleton::GetInstance<EngineSettings>().GetGameResolutionRation() = windowSize.x / (windowSize.y - 46);
		//const Math::Vector2i& resolution = Settings::SettingsRegistry::Get<Math::Vector2i>("graphics.resolution");

	}

	void GameWindow::UpdateSpecifiedRes()
	{
		ImVec2 windowSize = ImGui::GetWindowSize();

		glViewport(0, 0, windowSize.x, (windowSize.x * myWindowResAspect.y));

		float aspectRatio = (windowSize.x * myWindowResAspect.y) / windowSize.x;
		GraphicsEngine::UpdateGlobalUniform(UniformType::Float, "resolutionRatio", &aspectRatio);

		CommandListManager::GetSpriteCommandList().Execute();
		CommandListManager::GetUICommandList().Execute();
		if (myDrawGameGizmos) CommandListManager::GetDebugDrawCommandList().Execute();

		if (windowSize.x != myLastWindowResolution.x || windowSize.y != myLastWindowResolution.y)
		{
			glBindTexture(GL_TEXTURE_2D, myGameTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, (windowSize.x * myWindowResAspect.y), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);

			// Also update the framebuffer attachment
			glBindFramebuffer(GL_FRAMEBUFFER, myGameFrameBuffer);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myGameTexture, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		myLastWindowResolution = { static_cast<int>(windowSize.x), static_cast<int>(windowSize.y) };

		ImVec2 CursorPos = ImGui::GetCursorPos();
		CursorPos.x -= 8;
		CursorPos.y -= 7;

		if (windowSize.y - 46 > (windowSize.x * myWindowResAspect.y))
		{
			ImVec2 actuallWindowRes = windowSize;
			windowSize.y = (windowSize.x * myWindowResAspect.y);
			ImGui::SetCursorPos(ImVec2(CursorPos.x, actuallWindowRes.y * 0.5f - (windowSize.y - 46) * 0.5f));
		}
		else
		{
			ImVec2 actuallWindowRes = windowSize;
			windowSize.x = ((windowSize.y - 46) * myWindowResAspect.x);
			windowSize.y = windowSize.y - 46;
			ImGui::SetCursorPos(ImVec2(actuallWindowRes.x * 0.5f - windowSize.x * 0.5f, CursorPos.y));
		}
		//MainSingleton::GetInstance<EngineSettings>().GetGameResolution() = { (int)windowSize.x, (int)windowSize.y };
		Settings::GraphicsSettings::SetResolution(Math::Vector2i((int)windowSize.x, (int)windowSize.y));

		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
		float mousePosX = mousePos.x - cursorScreenPos.x;
		float mousePosY = windowSize.y - (mousePos.y - cursorScreenPos.y);
		Input::SetGamePosition({ static_cast<int>(mousePosX), static_cast<int>(mousePosY) });

		ImGui::Image(myGameTexture, windowSize, ImVec2(0, 1), ImVec2(1, 0));

		//MainSingleton::GetInstance<EngineSettings>().GetGameResolutionRation() = windowSize.x / windowSize.y;
	}

	void GameWindow::Open()
	{
		myCurrentWindowMode = static_cast<WindowMode>(currentResIndex);

		flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollWithMouse;



		glGenTextures(1, &myGameTexture);
		glGenFramebuffers(1, &myGameFrameBuffer);

		GraphicsEngine::BindFrameBuffer(myGameFrameBuffer);

		glBindTexture(GL_TEXTURE_2D, myGameTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myGameTexture, 0);

		GraphicsEngine::BindFrameBuffer(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}