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
            .size = sizeof(int)
        };

        return &type;
	}

    template<>
    inline TypeEntry* ReflectionRegistry::GetType<float>()
    {
        static TypeEntry type{
            .name = "float",
            .size = sizeof(float)
        };

        return &type;
    }

    template<>
    inline TypeEntry* ReflectionRegistry::GetType<bool>()
    {
        static TypeEntry type{
            .name = "bool",
            .size = sizeof(bool)
        };

        return &type;
    }
}