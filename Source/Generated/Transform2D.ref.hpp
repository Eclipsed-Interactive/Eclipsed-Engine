#pragma once

#include <cstddef>
#include "Reflection/Runtime/ReflectionRegistry.h"
#include "Reflection/Attributes/Attributes.hpp"
#include "Reflection/ReflectionRegistrar.hpp"

#include "C:/Users/zulto/Desktop/MyFiles/Projects/Eclipsed-Engine/Source/EclipsedEngine/EclipsedEngine/Components/Transform2D.h"

using namespace Eclipse;
using namespace Eclipse::Reflection;

template<> struct ReflectionRegistrar<Transform2D>
{
	static inline void Register()
	{
		Reflection::ReflectionRegistry::RegisterType(
			TypeEntry{
				.name = "Transform2D", 
				.size = sizeof(Transform2D),
				.id = Transform2D::GetReflectionID(),
				.create = [](){ return reinterpret_cast<void*>(new Transform2D); },
				.destroy = [](void* data){ delete reinterpret_cast<Transform2D*>(data); }
			}
		);

		{
			FieldEntry entry =
			{
				.name = "lastPosition",
				.offset = offsetof(Transform2D, lastPosition),
				.size = sizeof(Transform2D::lastPosition)
			};

			entry.attributeContainer.Add<SerializeField>();

			Reflection::ReflectionRegistry::RegisterField(
				"lastPosition",
				entry
			);
		}
	}
};
