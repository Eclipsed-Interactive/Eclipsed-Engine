#include "InspectorFieldDrawer.h"

#include "Reflection/Attributes/AttributeContainer.h"
#include "Reflection/Attributes/Attributes.hpp"

#include "Core/Math/Random.h"

#include "ImGui/imgui.h"

#include <Windows.h>
#include <string>

namespace Eclipse::Editor
{
	void InspectorFieldDrawer::Draw(void* v, const Reflection::FieldEntry& field)
	{
		if (!drawInspectorFuncs.contains(field.type))
		{
			NoDrawMethod();
			return;
		}

		drawInspectorFuncs[field.type](v, field);
	}

	void InspectorFieldDrawer::RegisterDefaults()
	{
		using namespace Reflection;

		Register<int>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 1.f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (range)
			{
				ImGui::DragInt("##value",
					(int*)addr,
					step,
					range->min,
					range->max
				);
			}
			else
			{
				ImGui::DragInt("##value", (int*)addr, step);
			}
			}
		);

		Register<float>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 0.1f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (range)
			{
				ImGui::DragFloat("##value",
					(float*)addr,
					step,
					range->min,
					range->max
				);
			}
			else
			{
				ImGui::DragFloat("##value", (float*)addr, step);
			}
			}
		);

		Register<bool>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			ImGui::Checkbox("##value", (bool*)addr);
			}
		);
	}

	void InspectorFieldDrawer::NoDrawMethod()
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.1f, 0.1f, 1.f));
		ImGui::Text(__FILE__);
		ImGui::Text("No draw method.");
		ImGui::PopStyleColor();
	}
}