//#include "ImGui/imgui.h"

#include "Reflection/Attributes/Attributes.hpp"
#include "Reflection/Attributes/AttributeContainer.h"

namespace Eclipse::Reflection
{
	template<typename T>
    inline TypeEntry* ReflectionRegistry::GetType()
	{
		// No type.
	}

	template<>
	inline TypeEntry* ReflectionRegistry::GetType<int>()
	{
        static TypeEntry type{
            .name = "int",
            .size = sizeof(int),
            .drawInspector = [](void* data, FieldEntry& field)
            {
                int& value = *static_cast<int*>(data);

                Range* range = field.attributeContainer.TryGet<Range>();
                if (range)
                {
                    Step<int>* step = field.attributeContainer.TryGet<Step<int>>();
                    int s = 1;
                    if (step)
                        s = step->step;

                    //ImGui::DragInt(
                    //    field.name.c_str(),
                    //    &value,
                    //    s,
                    //    range->min,
                    //    range->max
                    //);
                }
                else
                {
                    //ImGui::DragInt(field.name.c_str(), &value);
                }
            }
        };

        return &type;
	}

    template<>
    inline TypeEntry* ReflectionRegistry::GetType<float>()
    {
        static TypeEntry type{
            .name = "float",
            .size = sizeof(float),
            .drawInspector = [](void* data, FieldEntry& field)
            {
                float& value = *static_cast<float*>(data);

                Range* range = field.attributeContainer.TryGet<Range>();
                if (range)
                {
                    Step<float>* step = field.attributeContainer.TryGet<Step<float>>();
                    float s = 0.1f;
                    if (step)
                        s = step->step;

                    //ImGui::DragFloat(
                    //    field.name.c_str(),
                    //    &value,
                    //    s,
                    //    range->min,
                    //    range->max
                    //);
                }
                else
                {
                    //ImGui::DragFloat(field.name.c_str(), &value);
                }
            }
        };

        return &type;
    }

    template<>
    inline TypeEntry* ReflectionRegistry::GetType<bool>()
    {
        static TypeEntry type{
            .name = "bool",
            .size = sizeof(bool),
            .drawInspector = [](void* data, FieldEntry& field)
            {
                bool& value = *static_cast<bool*>(data);

                    //ImGui::DragFloat(
                    //    field.name.c_str(),
                    //    &value,
                    //    s,
                    //    range->min,
                    //    range->max
                    //);
            }
        };

        return &type;
    }
}