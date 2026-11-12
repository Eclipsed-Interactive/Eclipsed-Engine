#pragma once

#include <string>
#include <functional>
#include <stdexcept>

#include "Core/Standard/TypeIDGenerator.hpp"

#include "Serialization.Core.hpp"
#include "rapidjson/document.h"

#include <Reflection/Runtime/ReflectionRegistry.h>

namespace Eclipse::Serialization
{
	using ToJsonFunction = std::function<rapidjson::Value(const void*, rapidjson::Document::AllocatorType& allocator)>;
	using FromJsonFunction = std::function<void(void*, const rapidjson::Value&)>;

	using ToBinaryFunction = std::function<std::vector<std::byte>(const void*)>;

	struct JsonSerializer {
		ToJsonFunction toJson;
		FromJsonFunction fromJson;
	};

	class SERIALIZATION_API Serialization final
	{
	public:
		template<typename T>
		static void RegisterJson(rapidjson::Value(*toJson)(const T&, rapidjson::Document::AllocatorType& allocator), void(*fromJson)(void* v, const rapidjson::Value& sv));
		static void RegisterJson(unsigned long long typeId, JsonSerializer serializer);

		template<typename T>
		static const JsonSerializer& GetJson();
		static const JsonSerializer& GetJson(unsigned long long typeID);

		static const JsonSerializer* TryGetJson(TypeID type);

		static rapidjson::Value ContainerToJson(const void* v, const Reflection::ContainerOps* ops, rapidjson::Document::AllocatorType& allocator);
		static void ContainerFromJson(void* v, const Reflection::ContainerOps* ops, const rapidjson::Value& value);

		static rapidjson::Value ReflectedToJson(const void* v, const Reflection::TypeEntry* type, rapidjson::Document::AllocatorType& allocator);
		static void ReflectedFromJson(void* v, const Reflection::TypeEntry* type, const rapidjson::Value& value);

	private:
		static inline std::unordered_map<unsigned long long, JsonSerializer> jsonRegistry;
	};

	template<typename T>
	inline rapidjson::Value ToJson(const T& v, rapidjson::Document::AllocatorType& allocator)
	{
		return Serialization::GetJson<T>().toJson(reinterpret_cast<const void*>(&v), allocator);
	}

	inline rapidjson::Value ToJson(unsigned long long typeID, const void* v, rapidjson::Document::AllocatorType& allocator)
	{
		if (const JsonSerializer* s = Serialization::TryGetJson(typeID))
			return s->toJson(v, allocator);

		if (const Reflection::ContainerOps* ops = Reflection::ReflectionRegistry::TryGetContainer(typeID))
			return Serialization::ContainerToJson(v, ops, allocator);

		if (const Reflection::TypeEntry* type = Reflection::ReflectionRegistry::TryGetType(typeID))
			return Serialization::ReflectedToJson(v, type, allocator);

		throw std::runtime_error("No serializer for tpye.");
	}

	inline void FromJson(unsigned long long typeID, void* v, const rapidjson::Value& value)
	{
		if (const JsonSerializer* s = Serialization::TryGetJson(typeID))
			return s->fromJson(v, value);

		if (const Reflection::ContainerOps* ops = Reflection::ReflectionRegistry::TryGetContainer(typeID))
			return Serialization::ContainerFromJson(v, ops, value);

		if (const Reflection::TypeEntry* type = Reflection::ReflectionRegistry::TryGetType(typeID))
			return Serialization::ReflectedFromJson(v, type, value);

		throw std::runtime_error("No serializer for tpye.");
	}

	template<typename T>
	inline T FromJson(const rapidjson::Value& v)
	{
		T t{};
		return FromJson(Type::GetTypeID<T>(), &t, v);
	}

	template<typename T>
	inline void FromJson(T* t, std::string_view v)
	{
		//Serialization::GetJson<T>().fromJson(t, v);
	}

	template<typename T>
	inline std::string ToBinary(const T& v)
	{
		return {};
	}

	template<typename T>
	inline void Serialization::RegisterJson(rapidjson::Value(*toJson)(const T&, rapidjson::Document::AllocatorType&), void(*fromJson)(void* v, const rapidjson::Value& sv))
	{
		JsonSerializer serializer{
			.toJson = [toJson](const void* v, rapidjson::Document::AllocatorType& alloc) {return toJson(*reinterpret_cast<const T*>(v), alloc);},
			.fromJson = [fromJson](void* v,const rapidjson::Value& value) {return fromJson(v, value);},
		};

		RegisterJson(Type::GetTypeID<T>(), std::move(serializer));
	}

	template<typename T>
	inline const JsonSerializer& Serialization::GetJson()
	{
		return GetJson(Type::GetTypeID<T>());
	}
}