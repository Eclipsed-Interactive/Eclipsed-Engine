#include "Settings/Config.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <fstream>

#define CHECK_TYPE(_type) else if(val.type() == typeid(_type)) json[name.c_str()] = std::any_cast<_type>(val)

namespace Eclipse::Configs
{
	void Config::Save(const char* path)
	{

		rapidjson::Document doc;
		doc.SetObject();

		for (auto& [name, val] : values)
		{
			AddToJson<int>(val, doc, doc, name);
			AddToJson<float>(val, doc, doc, name);
			AddToJson<double>(val, doc, doc, name);
			AddToJson<bool>(val, doc, doc, name);
			AddToJson<std::string>(val, doc, doc, name);
			AddToJson<const char*>(val, doc, doc, name);
			AddToJson<Math::Vector2f>(val, doc, doc, name);
			AddToJson<Math::Vector2ui>(val, doc, doc, name);
			AddToJson<Math::Vector2i>(val, doc, doc, name);
			AddToJson<Math::Vector2d>(val, doc, doc, name);
		}

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::ofstream out(path);
		if (!out.is_open())
		{
			throw std::runtime_error(std::string("Cannot open file: ") + path);
		}

		out << buffer.GetString();
		out.close();
	}

	Config Config::Load(const char* path)
	{
		std::ifstream in(path);
		if (!in.is_open()) throw std::runtime_error("The file at path cant be opened.");

		std::string content((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		in.close();

		rapidjson::Document doc;
		doc.Parse(content.c_str());

		Config config;
		for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
		{
			std::string key = it->name.GetString();
			const auto& value = it->value;

			if (value.IsInt())
			{
				config.Add(key.c_str(), value.GetInt());
			}
			else if (value.IsFloat())
			{
				config.Add(key.c_str(), value.GetFloat());
			}
			else if (value.IsDouble())
			{
				config.Add(key.c_str(), value.GetDouble());
			}
			else if (value.IsBool())
			{
				config.Add(key.c_str(), value.GetBool());
			}
			else if (value.IsString())
			{
				config.Add(key.c_str(), std::string(value.GetString()));
			}
			else
			{
				const std::string& type = value["type"].GetString();
				const auto& val = value["val"];

				if (type == typeid(Math::Vector2f).name()) config.LoadVectorFromJson<Math::Vector2f>(key, val);
				else if (type == typeid(Math::Vector2i).name()) config.LoadVectorFromJson<Math::Vector2i>(key, val);
				else if (type == typeid(Math::Vector2ui).name()) config.LoadVectorFromJson<Math::Vector2ui>(key, val);
				else if (type == typeid(Math::Vector2d).name()) config.LoadVectorFromJson<Math::Vector2d>(key, val);
			}
		}

		return config;
	}

	void Config::GenerateHeaderFile()
	{
		std::string generated =
			"#pragma once\n"
			"#include \"CoreEngine/Math/Vector/Vector2.h\"\n"
			"namespace Eclipse::Configs\n"
			"{\n"
			+ std::string("class ") + name +
			"{\n";

		for (auto& [key, val] : values)
		{
			std::string type = "static inline const ";
			std::string stringValue = "";
			if (val.type() == typeid(int))
			{
				type += "int";
				stringValue = std::to_string(std::any_cast<int>(val));
			}
			else if (val.type() == typeid(float))
			{
				type += "float";
				stringValue = std::to_string(std::any_cast<float>(val)) + "f";
			}
			else if (val.type() == typeid(bool))
			{
				type += "bool";
				stringValue = std::any_cast<bool>(val) ? "true" : "false";
			}
			else if (val.type() == typeid(double)) 
			{
				type += "double";
				stringValue = std::to_string(std::any_cast<double>(val));
			}
			else if (val.type() == typeid(unsigned int))
			{
				type += "unsigned int";
				stringValue = std::to_string(std::any_cast<unsigned int>(val));
			}

			else if (val.type() == typeid(std::string))
			{
				type += "char*";
				stringValue += "\"";
				stringValue += std::any_cast<std::string>(val);
				stringValue += "\"";
			}

			else if (val.type() == typeid(const char*))
			{
				type += "char*";
				stringValue += "\"";
				stringValue += std::any_cast<const char*>(val);
				stringValue += "\"";
			}

			else if (val.type() == typeid(Math::Vector2f))
			{
				type += "Math::Vector2f";
				Math::Vector2f vec = std::any_cast<Math::Vector2f>(val);
				stringValue = "{ " + std::to_string(vec.x) + "f, " + std::to_string(vec.y) + "f};";
			}
			else if (val.type() == typeid(Math::Vector2d))
			{
				type += "Math::Vector2d";
				Math::Vector2d vec = std::any_cast<Math::Vector2d>(val);
				stringValue = "{ " + std::to_string(vec.x) + ", " + std::to_string(vec.y) + "};";
			}
			else if (val.type() == typeid(Math::Vector2ui))
			{
				type += "Math::Vector2ui";
				Math::Vector2ui vec = std::any_cast<Math::Vector2ui>(val);
				stringValue = "{ " + std::to_string(vec.x) + ", " + std::to_string(vec.y) + "};";
			}
			else if (val.type() == typeid(Math::Vector2i)) 
			{
				type += "Math::Vector2i";
				Math::Vector2i vec = std::any_cast<Math::Vector2i>(val);
				stringValue = "{ " + std::to_string(vec.x) + ", " + std::to_string(vec.y) + "};";
			}

			type += " ";
			type += key;
			type += "=";
			type += stringValue;
			type += ";\n";

			generated += type;
		}

		// add a lookup table method.

		generated += "};\n"
			"}\n";

		std::ofstream out(name + ".generated.h");
		out << generated.c_str();
	}
}