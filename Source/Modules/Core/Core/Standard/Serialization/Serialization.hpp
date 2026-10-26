#pragma once

#include <string>
#include <functional>
#include <stdexcept>

#include "Core/Standard/TypeIDGenerator.hpp"

#include "Core.Core.hpp"
#include "rapidjson/document.h"

namespace Eclipse::Serialization
{
	using ToJsonFunction = std::function<rapidjson::Value(const void*, rapidjson::Document::AllocatorType& allocator)>;
	using FromJsonFunction = std::function<void(void*, const rapidjson::Value&)>;

	using ToBinaryFunction = std::function<std::vector<std::byte>(const void*)>;

	struct JsonSerializer {
		ToJsonFunction toJson;
		FromJsonFunction fromJson;
	};

	class CORE_API Serialization final
	{
	public:
		template<typename T>
		static void RegisterJson(rapidjson::Value(*toJson)(const T&, rapidjson::Document::AllocatorType& allocator), void(*fromJson)(void* v, const rapidjson::Value& sv));

		template<typename T>
		static const JsonSerializer& GetJson();
		static const JsonSerializer& GetJson(unsigned long long typeID);

	private:
		static inline std::unordered_map<unsigned long long, JsonSerializer> jsonRegistry;
	};

	template<typename T>
	inline rapidjson::Value ToJson(const T& v, rapidjson::Document::AllocatorType& allocator)
	{
		return Serialization::GetJson<T>().toJson(reinterpret_cast<const void*>(&v), allocator);
	}

	inline rapidjson::Value ToJson(unsigned long long typeID, void* v, rapidjson::Document::AllocatorType& allocator)
	{
		return Serialization::GetJson(typeID).toJson(v, allocator);
	}

	inline void FromJson(unsigned long long typeID, void* v, const rapidjson::Value& value)
	{
		return Serialization::GetJson(typeID).fromJson(v, value);
	}

	template<typename T>
	inline T FromJson(const rapidjson::Value& v)
	{
		T t{};
		//Serialization::GetJson<T>().fromJson(&t, v);

		return t;
	}

	template<typename T>
	inline void FromJson(T* t, std::string_view v)
	{
		//Serialization::GetJson<T>().fromJson(t, v);
	}

	template<typename T>
	inline std::string ToBinary(const T& v)
	{

	}

	template<typename T>
	inline void Serialization::RegisterJson(rapidjson::Value(*toJson)(const T&, rapidjson::Document::AllocatorType&), void(*fromJson)(void* v, const rapidjson::Value& sv))
	{
		JsonSerializer serializer{
			.toJson = [toJson](const void* v, rapidjson::Document::AllocatorType& alloc) {return toJson(*reinterpret_cast<const T*>(v), alloc);},
			.fromJson = [fromJson](void* v,const rapidjson::Value& value) {return fromJson(v, value);},
		};

		jsonRegistry.insert_or_assign(Type::GetTypeID<T>(), serializer);
	}

	template<typename T>
	inline const JsonSerializer& Serialization::GetJson()
	{
		TypeID id = Type::GetTypeID<T>();
		if (!jsonRegistry.contains(id))
			throw std::runtime_error("Serializer not registered.");

		return jsonRegistry.at(id);
	}
}