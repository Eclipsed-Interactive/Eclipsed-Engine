#include "InspectorFieldDrawer.h"

#include "Reflection/Attributes/AttributeContainer.h"
#include "Reflection/Attributes/Attributes.hpp"

#include "Core/Math/Random.h"

#include "ImGui/imgui.h"

namespace Eclipse::Editor
{
	void InspectorFieldDrawer::Draw(void* v, const Reflection::FieldEntry& field)
	{
		if (!drawInspectorFuncs.contains(field.type))
			return;

		drawInspectorFuncs[field.type](&v, field);
	}

	void InspectorFieldDrawer::RegisterDefaults()
	{
		using namespace Reflection;

		Register<int>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();
			const int randomID = Random::Rand<int>();

			const std::string id = std::to_string(randomID);

			int step = 1;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			if (range)
			{
				ImGui::DragInt(("##" + id + entry.name).c_str(),
					(int*)v,
					step,
					range->min,
					range->max
				);
			}
			else
			{
				ImGui::DragInt(("##" + id + entry.name).c_str(), (int*)v, step);
			}
			}
		);

		Register<float>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();
			const int randomID = Random::Rand<int>();

			const std::string id = std::to_string(randomID);

			float step = 0.1f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			if (range)
			{
				ImGui::DragFloat(("##" + id + entry.name).c_str(),
					(float*)v,
					step,
					range->min,
					range->max
				);
			}
			else
			{
				ImGui::DragFloat(("##" + id + entry.name).c_str(), (float*)v, step);
			}
			}
		);

		Register<bool>([](void* v, const FieldEntry& entry) {
			const int randomID = Random::Rand<int>();

			const std::string id = std::to_string(randomID);

			ImGui::Checkbox(("##" + id + entry.name).c_str(), (bool*)v);
			}
		);
	}
}