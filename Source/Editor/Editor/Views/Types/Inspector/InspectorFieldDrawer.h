#pragma once

#include <unordered_map>
#include <functional>

#include "Core/Standard/TypeIDGenerator.hpp"

#include "Reflection/Runtime/ReflectionRegistry.h"

namespace Eclipse::Editor 
{
	class InspectorFieldDrawer
	{
	public:
		template<typename T>
		static void Draw(T& v, const Reflection::FieldEntry& field);

		static void Draw(void* v, const Reflection::FieldEntry& field);

		static void RegisterDefaults();

	private:
		template<typename T>
		static void Register(std::function<void(void*, const Reflection::FieldEntry&)> drawInspectorFunc);

		static void NoDrawMethod();

	private:
		static inline std::unordered_map<TypeID, std::function<void(void*, const Reflection::FieldEntry&)>> drawInspectorFuncs;
	};

	template<typename T>
	inline void InspectorFieldDrawer::Draw(T& v, const Reflection::FieldEntry& field)
	{
		const TypeID id = Type::GetTypeID<T>();
		if (!drawInspectorFuncs.contains(id))
		{
			NoDrawMethod();
			return;
		}

		drawInspectorFuncs[id](&v, field);
	}

	template<typename T>
	inline void InspectorFieldDrawer::Register(std::function<void(void*, const Reflection::FieldEntry&)> drawInspectorFunc)
	{
		drawInspectorFuncs.insert_or_assign(Type::GetTypeID<T>(), drawInspectorFunc);
	}
}