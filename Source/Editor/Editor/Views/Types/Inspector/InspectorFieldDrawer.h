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
		static bool DrawValue(void* value, TypeID type, const Reflection::AttributeContainer& attrs);

	private:
		template<typename T>
		static void Register(std::function<bool(void*, const Reflection::AttributeContainer&)> drawInspectorFunc);

		static void NoDrawMethod();


		template<typename T>
		static bool DrawNumerical(void* v, const Reflection::AttributeContainer& attrs);

		template<typename... Ts>
		static void RegisterNumeric() { (Register<Ts>(&DrawNumerical<Ts>), ...); }

		static bool DrawContainer(void* container, const Reflection::ContainerOps& ops, const Reflection::AttributeContainer& attrs);

	private:
		static inline std::unordered_map<TypeID, std::function<bool(void*, const Reflection::AttributeContainer&)>> drawInspectorFuncs;
	};

	template<typename T>
	inline void InspectorFieldDrawer::Draw(T& v, const Reflection::FieldEntry& field)
	{
		unsigned char* object = reinterpret_cast<unsigned char*>(v) + field.offset;

		if (DrawValue(object, field.type, field.attributeContainer))
		{
			InvokeOnChange(v, field);
			return;
		}
	}

	template<typename T>
	inline void InspectorFieldDrawer::Register(std::function<bool(void*, const Reflection::AttributeContainer&)> drawInspectorFunc)
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
	inline bool InspectorFieldDrawer::DrawNumerical(void* v, const Reflection::AttributeContainer& attrs)
	{
		using namespace Reflection;

		using Scalar = typename NumericTraits<T>::Scalar;
		constexpr int Count = NumericTraits<T>::Count;

		Scalar min = 0, max = 0;

		const Range* range = attrs.TryGet<Range>();
		if (range)
		{
			min = (Scalar)range->min;
			max = (Scalar)range->max;
		}

		float step = std::is_floating_point_v<Scalar> ? 0.001f : 1;
		const Step* stepAttr = attrs.TryGet<Step>();
		if (stepAttr) step = stepAttr->step;

		const bool changed = range 
			? ImGui::SliderScalarN("##value", ToImGuiDataType<Scalar>(), v, Count, &min, &max)
			: ImGui::DragScalarN("##value", ToImGuiDataType<Scalar>(), v, Count, step);

		return changed;
	}
}