#include "Config.h"

#include <any>
#include <stdexcept>

namespace Eclipse::Configs
{
	template<typename T>
	inline void Config::Add(const char* key, const T& defaultValue)
	{
		values[std::string(key)] = defaultValue;
	}
	
    template<typename T>
    inline T Config::Get(const char* key)
    {
        std::string sKey = std::string(key);
        return std::any_cast<T>(values[sKey]);
    }

    template<typename T>
	inline void Config::AddToJson(const std::any& val, rapidjson::Document& doc, rapidjson::Value& obj, const std::string& name)
	{
		if (val.type() == typeid(T))
		{
            rapidjson::Value key(name.c_str(), doc.GetAllocator());
            rapidjson::Value value;

            if constexpr (std::is_same_v<T, int>)
                value.SetInt(std::any_cast<int>(val));
            else if constexpr (std::is_same_v<T, float>)
                value.SetFloat(std::any_cast<float>(val));
            else if constexpr (std::is_same_v<T, double>)
                value.SetDouble(std::any_cast<double>(val));
            else if constexpr (std::is_same_v<T, bool>)
                value.SetBool(std::any_cast<bool>(val));
            else if constexpr (std::is_same_v<T, std::string>)
                value.SetString(std::any_cast<std::string>(val).c_str(), doc.GetAllocator());
            else if constexpr (std::is_same_v<T, const char*>)
                value.SetString(std::any_cast<const char*>(val), doc.GetAllocator());

            else if constexpr (std::is_same_v<T, Math::Vector2f>)
                AddVectorToJson<Math::Vector2f>(value, val, doc.GetAllocator());
            else if constexpr (std::is_same_v<T, Math::Vector2i>)
                AddVectorToJson<Math::Vector2i>(value, val, doc.GetAllocator());
            else if constexpr (std::is_same_v<T, Math::Vector2ui>)
                AddVectorToJson<Math::Vector2ui>(value, val, doc.GetAllocator());
            else if constexpr (std::is_same_v<T, Math::Vector2d>)
                AddVectorToJson<Math::Vector2d>(value, val, doc.GetAllocator());

            else
                throw std::runtime_error("The type is not added.");

            doc.AddMember(key.Move(), value.Move(), doc.GetAllocator());
		}
	}
    template<typename T>
    inline void Config::AddVectorToJson(rapidjson::Value& value, const std::any& val, rapidjson::Document::AllocatorType& allocator)
    {
        value.SetObject();
        rapidjson::Value name(typeid(T).name(), allocator);
        value.AddMember("type", name, allocator);

        rapidjson::Value vecVal(rapidjson::kObjectType);

        T vec = std::any_cast<T>(val);
        vecVal.AddMember("x", vec.x, allocator);
        vecVal.AddMember("y", vec.y, allocator);

        value.AddMember("val", vecVal, allocator);
    }

    template<typename T>
    inline void Config::LoadVectorFromJson(const std::string& name, const rapidjson::Value& value)
    {
        T vec;

        if (typeid(Math::Vector2i).name() == typeid(T).name())
        {
            vec.x = value["x"].GetInt();
            vec.y = value["y"].GetInt();
        }
        else if (typeid(Math::Vector2d).name() == typeid(T).name())
        {
            vec.x = value["x"].GetDouble();
            vec.y = value["y"].GetDouble();
        }
        else if (typeid(Math::Vector2f).name() == typeid(T).name())
        {
            vec.x = value["x"].GetFloat();
            vec.y = value["y"].GetFloat();
        }
        else if (typeid(Math::Vector2ui).name() == typeid(T).name())
        {
            vec.x = value["x"].GetUint();
            vec.y = value["y"].GetUint();
        }

        values[name] = vec;
    }
}