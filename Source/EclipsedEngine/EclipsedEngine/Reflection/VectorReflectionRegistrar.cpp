#include "Reflection/Runtime/ReflectionRegistry.h"

#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Vector/Vector3.h"
#include "Core/Math/Vector/Vector4.h"

namespace {
	template<typename V, typename T>
	void RegisterVector(const char* aName, std::initializer_list<const char*> axes)
	{
		using namespace Eclipse;
		using namespace Eclipse::Reflection;

		TypeEntry type = TypeEntry{
			.name = aName,
			.size = sizeof(V),
			.id = Type::GetTypeID<V>(),
			.create = []() { return static_cast<void*>(new V); },
			.createAtAddress = [](unsigned char* addr) { return static_cast<void*>(new (addr) V); },
			.destroy = [](void* data) { delete static_cast<V*>(data); },
			.flag = ReflectionFlags::Serializable,
		};

		for (const char* axis : axes)
			type.fieldOrder.emplace_back(axis);

		ReflectionRegistry::RegisterType(std::move(type));

		size_t index = 0;
		for (const char* axis : axes)
		{
			ReflectionRegistry::RegisterField(aName, FieldEntry{
				.name = axis,
				.owner = aName,
				.offset = index++ * sizeof(T),
				.size = sizeof(T),
				.type = Type::GetTypeID<T>()
				});
		}
	}

	template<template<typename> class MathContainer>
	void RegisterAll(const char* prefix, std::initializer_list<const char*> axes)
	{
		const std::string p = prefix;
		RegisterVector<MathContainer<int>, int>((p + "i").c_str(), axes);
		RegisterVector<MathContainer<unsigned int>, unsigned int>((p + "ui").c_str(), axes);

		RegisterVector<MathContainer<float>, float>((p + "f").c_str(), axes);
		RegisterVector<MathContainer<double>, double>((p + "d").c_str(), axes);

		RegisterVector<MathContainer<long>, long>((p + "l").c_str(), axes);
		RegisterVector<MathContainer<unsigned long >, unsigned long >((p + "ul").c_str(), axes);

		RegisterVector<MathContainer<long long>, long long>((p + "ll").c_str(), axes);
		RegisterVector<MathContainer<unsigned long long>, unsigned long long>((p + "ull").c_str(), axes);
	}

	struct MathTypesRegistrar {
		MathTypesRegistrar()
		{
			using namespace Eclipse;
			RegisterAll<Math::Vector2>("Vector2", { "x", "y" });
			RegisterAll<Math::Vector3>("Vector3", { "x", "y", "z" });
			RegisterAll<Math::Vector4>("Vector4", { "x", "y", "z", "w" });
		}
	};

	MathTypesRegistrar _mathTypesRegistrar;
}