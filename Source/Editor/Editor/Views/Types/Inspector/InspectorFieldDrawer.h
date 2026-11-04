#pragma once

#include <unordered_map>
#include <functional>

#include "Core/Standard/TypeIDGenerator.hpp"

#include "Reflection/Runtime/ReflectionRegistry.h"

#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Vector/Vector3.h"
#include "Core/Math/Vector/Vector4.h"

#include "Reflection/Reflection.hpp"
#include "ImGui/imgui.h"


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


		template<typename T>
		static void DrawNumerical(void* v, const Reflection::FieldEntry& field);

		template<typename... Ts>
		static void RegisterNumeric() { (Register<Ts>(&DrawNumerical<Ts>), ...); }

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

	template<typename T> struct NumericTraits					{ using Scalar = T;  static constexpr int Count = 1; };
	template<typename T> struct NumericTraits<Math::Vector2<T>> { using Scalar = T;  static constexpr int Count = 2; };
	template<typename T> struct NumericTraits<Math::Vector3<T>> { using Scalar = T;  static constexpr int Count = 3; };
	template<typename T> struct NumericTraits<Math::Vector4<T>> { using Scalar = T;  static constexpr int Count = 4; };

	template<typename T>
	constexpr ImGuiDataType ToImGuiDataType()
	{
		if constexpr (std::is_same_v<T, float>) return ImGuiDataType_Float;
		else if constexpr (std::is_same_v<T, double>) return ImGuiDataType_Double;
		else if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>)
		{
			constexpr bool _signed = std::is_signed_v<T>;
			if constexpr (sizeof(T) == 1)		return _signed ? ImGuiDataType_S8 : ImGuiDataType_U8;
			else if constexpr (sizeof(T) == 2)	return _signed ? ImGuiDataType_S16 : ImGuiDataType_U16;
			else if constexpr (sizeof(T) == 4)	return _signed ? ImGuiDataType_S32 : ImGuiDataType_U32;
			else								return _signed ? ImGuiDataType_S64 : ImGuiDataType_U64;
		}
		else static_assert(sizeof(T) == 0, "Unsupported numeric type.");
	}

	template<typename T>
	inline void InspectorFieldDrawer::DrawNumerical(void* v, const Reflection::FieldEntry& field)
	{
		using namespace Reflection;

		using Scalar = typename NumericTraits<T>::Scalar;
		constexpr int Count = NumericTraits<T>::Count;

		const Range* range = field.attributeContainer.TryGet<Range>();

		float step = std::is_floating_point_v<Scalar> ? 0.001f : 1;
		const Step* stepAttr = field.attributeContainer.TryGet<Step>();
		if (stepAttr) step = stepAttr->step;

		unsigned char* addr = reinterpret_cast<unsigned char*>(v) + field.offset;
		T data = *reinterpret_cast<T*>(addr);

		const bool changed = range 
			? ImGui::SliderScalarN("##value", ToImGuiDataType<Scalar>(), &data, Count, &range->min, &range->max)
			: ImGui::DragScalarN("##value", ToImGuiDataType<Scalar>(), &data, Count, step);

		if (changed) Set(field.owner, field.name, v, data);
	}
}