#include "Serialization.hpp"

namespace Eclipse::Serialization
{
	void Serialization::RegisterJson(unsigned long long typeId, JsonSerializer serializer)
	{
		jsonRegistry.insert_or_assign(typeId, std::move(serializer));
	}

	const JsonSerializer* Serialization::TryGetJson(TypeID type)
	{
		auto it = jsonRegistry.find(type);
		return it != jsonRegistry.end() ? &it->second : nullptr;
	}

	rapidjson::Value Serialization::ContainerToJson(const void* v, const Reflection::ContainerOps* ops, rapidjson::Document::AllocatorType& allocator)
	{
		const size_t count = ops->size(v);
		rapidjson::Value arr(rapidjson::kArrayType);
		arr.Reserve(count, allocator);

		for (size_t i = 0; i < count; i++)
		{
			const void* element = ops->atConst(v, i);
			rapidjson::Value val = ToJson(ops->elementType, element, allocator);
			arr.PushBack(val, allocator);
		}

		return arr;
	}

	void Serialization::ContainerFromJson(void* v, const Reflection::ContainerOps* ops, const rapidjson::Value& value)
	{
		if (!value.IsArray()) return;

		ops->resize(v, 0);
		ops->resize(v, value.Size());

		for (size_t i = 0; i < value.Size(); i++)
		{
			FromJson(ops->elementType, ops->at(v, i), value[i]);
		}
	}

	rapidjson::Value Serialization::ReflectedToJson(const void* v, const Reflection::TypeEntry* type, rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value obj(rapidjson::kObjectType);
		for (const std::string& name : type->fieldOrder)
		{
			const Reflection::FieldEntry& field = type->fields.at(name);
			const void* fieldPtr = static_cast<const unsigned char*>(v) + field.offset;

			rapidjson::Value key(name.c_str(), name.size());
			rapidjson::Value val = ToJson(field.type, fieldPtr, allocator);

			obj.AddMember(key, val, allocator);
		}

		return obj;
	}

	void Serialization::ReflectedFromJson(void* v, const Reflection::TypeEntry* type, const rapidjson::Value& value)
	{
		if (!value.IsObject()) return;

		for (const std::string& name : type->fieldOrder)
		{
			auto member = value.FindMember(name.c_str());
			if (member == value.MemberEnd())
				continue;

			const Reflection::FieldEntry& field = type->fields.at(name);
			void* fieldPtr = static_cast<unsigned char*>(v) + field.offset;
			FromJson(field.type, fieldPtr, member->value);
		}
	}

	inline const JsonSerializer& Serialization::GetJson(unsigned long long typeID)
	{
		if (!jsonRegistry.contains(typeID))
			throw std::runtime_error("Serializer not registered.");

		return jsonRegistry.at(typeID);
	}
}