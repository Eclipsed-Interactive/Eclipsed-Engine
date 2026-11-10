#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Reflection/Attributes/AttributeContainer.h"
#include "Core/Standard/TypeIDGenerator.hpp"

#include "ReflectionFlags.h"

#include "Reflection.Core.hpp"
#include "Reflection/TypeEntry.h"
#include "Reflection/FieldEntry.h"
#include "Reflection/ContainerOps.h"

#include <Core/Standard/Traits/VectorTraits.hpp>

namespace Eclipse::Reflection
{
	class REFLECTION_API ReflectionRegistry
	{
	public:
		static void RegisterType(TypeEntry type);
		static void RegisterField(const std::string& typeName, FieldEntry field);

		static size_t GetOffset(const std::string& typeName, const std::string& fieldName);

	public:
		static const FieldEntry& GetField(const std::string& typeName, const std::string& fieldName);
		static const TypeEntry& GetType(const std::string& typeName);

		static const std::unordered_map<std::string, TypeEntry>& GetTypes();

		static const TypeEntry* TryGetType(const std::string& typeName);

	public:
		template<typename T>
		static TypeEntry* GetType();

		template<typename T>
		static void RegisterType();

	private:
		static void RegisterContainer(TypeID aTypeId, ContainerOps aContainerOps);

	private:
		static inline std::unordered_map<TypeID, ContainerOps> containers;
		static inline std::unordered_map<std::string, TypeEntry> registererd;
	};
}



namespace Eclipse::Reflection
{
	template<typename T>
	inline void ReflectionRegistry::RegisterType()
	{
		using Trait = Traits::TypeTrait<std::remove_cv_t<T>>;

		if constexpr (Traits::is_vector_v<T>)
		{
			using Vector = Trait::Container;
			using Element = Trait::Element;

			Vector d;
			Element f;

			RegisterContainer(Type::GetTypeID<Vector>(), ContainerOps{
				.elementType = Type::GetTypeID<Element>(),
				.size = [](const void* container) -> size_t { return static_cast<const Vector*>(container)->size(); },
				.at = [](void* container, size_t i) -> void* { return &(*static_cast<Vector*>(container))[i]; },
				});

			RegisterType<Element>();
		}
	}

	//template<typename T>
	//inline void ReflectionRegistry::RegisterType()
	//{
	//	using Trait = Traits::TypeTrait<T>;

	//	if constexpr (Traits::is_vector_v<T>)
	//	{
	//		using Vector = Trait::Container;
	//		using Type = Trait::Element;

	//		RegisterContainer(TypeID::GetTypeID<T>(), ContainerOps{
	//			.elementType = TypeID::GetTypeID<Trait::Element>(),
	//			.size = [](const void* container) -> size_t {return static_cast<const Vector*>(container)->size();}
	//			});

	//		RegisterType<Type>();
	//	}
	//}
}