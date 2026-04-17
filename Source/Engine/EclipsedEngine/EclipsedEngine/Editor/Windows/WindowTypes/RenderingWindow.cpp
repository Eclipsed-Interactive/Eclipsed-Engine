#include "RenderingWindow.h"

#include "EclipsedEngine/Components/UI/TextRenderer.h"
#include "EclipsedEngine/Components/Rendering/Camera.h"
#include "EclipsedEngine/Components/UI/Canvas.h"

#include "PhysicsEngine/PhysicsEngine.h"

	void Eclipse::Editor::BaseRenderingWindow::DrawGizmoPopup()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Always);
		if (ImGui::BeginPopup("GizmoPicker", NULL))
		{
			if (ImGui::CollapsingHeader("Rects"))
			{
				ImGui::Checkbox("Draw Text Renderer Rects##DrawTextRenderDebugLines", &TextRenderer::drawRectGizmos);
				ImGui::Checkbox("Draw Camera Gizmos##DrawCameraGizmosDebugLines", &Camera::drawCameraGizmos);
				ImGui::Checkbox("Draw Canvas Gizmos##DrawCanvasGizmosDebugLines", &Canvas::drawCanvasGizmos);
			}
			if (ImGui::CollapsingHeader("Physics"))
			{
				ImGui::Indent(20.f);
				ImGui::BeginChild(27491898, ImVec2(0, 0), ImGuiChildFlags_Borders);
				ImGui::Checkbox("Draw Physics Debug Lines##DrawAnyPhysicsDebugLines", &PhysicsEngine::GetDebugDraw());
				ImGui::Checkbox("Draw Shapes##DebugDrawShapes", &PhysicsEngine::GetDebugDrawShapes(drawShapes));
				ImGui::Checkbox("Draw Queries##DebugDrawQueries", &PhysicsEngine::GetDebugDrawShapes(drawQueries));
				ImGui::Checkbox("Draw Joints##DebugDrawJoints", &PhysicsEngine::GetDebugDrawShapes(drawJoints));
				ImGui::Checkbox("Draw Joints Advanced##DebugDrawJointsAdvanced", &PhysicsEngine::GetDebugDrawShapes(drawJointExtras));
				ImGui::Checkbox("Draw Bounds##DebugDrawBounds", &PhysicsEngine::GetDebugDrawShapes(drawBounds));
				ImGui::Checkbox("Draw Mass##DebugDrawMass", &PhysicsEngine::GetDebugDrawShapes(drawMass));
				ImGui::Checkbox("Draw Body Names##DebugDrawBodyNames", &PhysicsEngine::GetDebugDrawShapes(drawBodyNames));
				ImGui::Checkbox("Draw Contacts##DebugDrawContacts", &PhysicsEngine::GetDebugDrawShapes(drawContacts));
				ImGui::Checkbox("Draw Graph Colors##DebugDrawGraphColors", &PhysicsEngine::GetDebugDrawShapes(drawGraphColors));
				ImGui::Checkbox("Draw Contact Normals##DebugDrawContactNormals", &PhysicsEngine::GetDebugDrawShapes(drawContactNormals));
				ImGui::Checkbox("Draw Contact Impulses##DebugDrawContactImpulses", &PhysicsEngine::GetDebugDrawShapes(drawContactImpulses));
				ImGui::Checkbox("Draw Contact Features##DebugDrawContactFeatures", &PhysicsEngine::GetDebugDrawShapes(drawContactFeatures));
				ImGui::Checkbox("Draw Friction Impulse##DebugDrawFrictionImpulse", &PhysicsEngine::GetDebugDrawShapes(drawFrictionImpulses));
				ImGui::Checkbox("Draw Islands##DebugDrawIslands", &PhysicsEngine::GetDebugDrawShapes(drawIslands));
				ImGui::EndChild();
				ImGui::Unindent();
			}
			ImGui::EndPopup();
		}
	}
	void Eclipse::Editor::BaseRenderingWindow::DrawGizmoButtons(bool& drawGizmo)
	{
		const float ArrowButtonOffset = 9.f;
		const float GizmoButtonsOffset = 32.f;

		const float menu_bar_width = ImGui::GetWindowContentRegionMax().x;
		const float button_width = ImGui::CalcTextSize("Gizmo").x + ImGui::GetStyle().FramePadding.x * 2;
		ImGui::Dummy({ menu_bar_width - button_width - GizmoButtonsOffset, 0 });
		ImGui::SameLine();

		ImVec4 color = drawGizmo ? ImVec4(1.f, 0.176f, 0.176f, 1.f) : ImVec4(0.196f, 1.f, 0.314f, 1.f);
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.f, 0.f, 1.f));

		if (ImGui::Button("Gizmo"))
		{
			drawGizmo = !drawGizmo;
		}
		ImGui::PopStyleColor(2);


		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ArrowButtonOffset);

		if (ImGui::ArrowButton("##gizmo_selection", ImGuiDir_Down))
		{
			ImGui::OpenPopup("GizmoPicker");
		}

		DrawGizmoPopup();
	}
