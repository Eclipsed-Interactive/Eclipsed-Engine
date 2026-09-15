#include "InputMapper.h"

#include <xhash>
#include <string>
#include "Input.h"

#include "Core/PathManager.h"

#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace Eclipse::Input
{
	std::unordered_map<std::string, InputMapper> InputMapper::mappings;

	InputMapper* InputMapper::Load(std::string_view path)
	{
		std::string spath = std::string(path);
		std::ifstream file(spath);

		if (!file.is_open())
			return nullptr;

		std::string json(
			(std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>()
		);

		file.close();

		rapidjson::Document document;
		document.Parse(json.c_str());

		if (document.HasParseError() || !document.IsObject())
			return nullptr;

		InputMapper* mapper = new InputMapper();

		if (document.HasMember("axes") &&
			document["axes"].IsArray())
		{
			for (const auto& value : document["axes"].GetArray())
			{
				if (!value.IsObject())
					continue;

				if (!value.HasMember("name") ||
					!value["name"].IsString())
					continue;

				if (!value.HasMember("id") ||
					!value["id"].IsUint64())
					continue;

				if (!value.HasMember("positive") ||
					!value["positive"].IsInt())
					continue;

				if (!value.HasMember("negative") ||
					!value["negative"].IsInt())
					continue;

				Axis axis;

				axis.Name = value["name"].GetString();
				axis.ID = value["id"].GetUint64();

				axis.Positive =
					static_cast<Keycode::Scancode>(
						value["positive"].GetInt()
						);

				axis.Negative =
					static_cast<Keycode::Scancode>(
						value["negative"].GetInt()
						);

				mapper->registeredAxis.emplace(axis.ID, axis);
				mapper->axisNames.emplace(axis.Name, axis.ID);
			}
		}

		if (document.HasMember("actions") &&
			document["actions"].IsArray())
		{
			for (const auto& value : document["actions"].GetArray())
			{
				if (!value.IsObject())
					continue;

				if (!value.HasMember("name") ||
					!value["name"].IsString())
					continue;

				if (!value.HasMember("id") ||
					!value["id"].IsUint64())
					continue;

				if (!value.HasMember("keys") ||
					!value["keys"].IsArray())
					continue;

				Action action;

				action.Name = value["name"].GetString();
				action.ID = value["id"].GetUint64();

				for (const auto& key : value["keys"].GetArray())
				{
					if (!key.IsInt())
						continue;

					action.Keys.push_back(
						static_cast<Keycode::Scancode>(
							key.GetInt()
							)
					);
				}

				mapper->registeredActions.emplace(action.ID, action);
				mapper->actionNames.emplace(action.Name, action.ID);
			}
		}

		return mapper;
	}

	void InputMapper::Save()
	{
		using namespace rapidjson;

		Document document;
		document.SetObject();

		auto& allocator = document.GetAllocator();

		Value axes(kArrayType);

		for (const auto& [id, axis] : registeredAxis)
		{
			Value axisObject(kObjectType);

			axisObject.AddMember(
				"name",
				Value(axis.Name.c_str(), allocator),
				allocator
			);

			axisObject.AddMember(
				"id",
				axis.ID,
				allocator
			);

			axisObject.AddMember(
				"positive",
				static_cast<int>(axis.Positive),
				allocator
			);

			axisObject.AddMember(
				"negative",
				static_cast<int>(axis.Negative),
				allocator
			);

			axes.PushBack(axisObject, allocator);
		}

		document.AddMember("axes", axes, allocator);

		Value actions(kArrayType);

		for (const auto& [id, action] : registeredActions)
		{
			Value actionObject(kObjectType);

			actionObject.AddMember(
				"name",
				Value(action.Name.c_str(), allocator),
				allocator
			);

			actionObject.AddMember(
				"id",
				action.ID,
				allocator
			);

			Value keys(kArrayType);

			for (const auto key : action.Keys)
			{
				keys.PushBack(
					static_cast<int>(key),
					allocator
				);
			}

			actionObject.AddMember("keys", keys, allocator);

			actions.PushBack(actionObject, allocator);
		}

		document.AddMember("actions", actions, allocator);

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);

		document.Accept(writer);

		std::ofstream file(path, std::ios::out | std::ios::trunc);

		if (!file.is_open())
			return;

		file << buffer.GetString();
	}

	InputMapper* InputMapper::CreateMapping(std::string_view mapName)
	{
		mappings[std::string(mapName)] = {};
		InputMapper& map = mappings[std::string(mapName)];
		map.path = (PathManager::GetAssetsPath() / (std::string(mapName) + ".input")).generic_string();

		return &map;
	}

	std::unordered_map<std::string, InputMapper>& InputMapper::GetMappings()
	{
		return mappings;
	}

	InputMapper* InputMapper::GetMapping(std::string_view mapName)
	{
		auto it = mappings.find(std::string(mapName));
		if (it == mappings.end())
			return CreateMapping(mapName);

		return &it->second;
	}

	unsigned long long InputMapper::CreateAxis(std::string_view axisName, Keycode::Scancode positive, Keycode::Scancode negative)
	{
		unsigned long long id = std::hash<std::string_view>{}(axisName);
		registeredAxis[id] = Axis{
			.Name = std::string(axisName),
			.ID = id,
			.Positive = positive,
			.Negative = negative
		};

		axisNames[std::string(axisName)] = id;

		return id;
	}

	unsigned long long InputMapper::CreateAction(std::string_view actionName, Keycode::Scancode key)
	{
		unsigned long long id = std::hash<std::string_view>{}(actionName);

		registeredActions[id] = Action{
			.Name = std::string(actionName),
			.ID = id
		};

		actionNames[std::string(actionName)] = id;

		if (key != Keycode::UNKNOWN)
			Bind(actionName, key);

		return id;
	}

	void InputMapper::Bind(std::string_view actionName, Keycode::Scancode key)
	{
		Action& action = FindAction(actionName);
		action.Keys.push_back(key);
	}

	bool InputMapper::WasPressed(std::string_view actionName)
	{
		const Action& action = FindAction(actionName);
		return IsActionPressed(action, ActionState::WasPressed);
	}

	bool InputMapper::IsPressed(std::string_view actionName)
	{
		const Action& action = FindAction(actionName);
		return IsActionPressed(action, ActionState::IsPressed);
	}

	bool InputMapper::WasReleased(std::string_view actionName)
	{
		const Action& action = FindAction(actionName);
		return IsActionPressed(action, ActionState::WasReleased);
	}

	int InputMapper::GetAxis(std::string_view axisName)
	{
		const Axis& axis = FindAxis(axisName);
		return (int)Input::GetKeyDown(axis.Positive) - (int)Input::GetKeyDown(axis.Negative);
	}

	Axis& InputMapper::FindAxis(std::string_view axisName)
	{
		static Axis null_axis{ .Name = "NULL", .ID = 0, .Positive = Keycode::UNKNOWN, .Negative = Keycode::UNKNOWN };

		auto it = axisNames.find(std::string(axisName));

		if (it == axisNames.end())
			return null_axis;

		const unsigned long long id = it->second;


		auto axisIt = registeredAxis.find(id);

		if (axisIt == registeredAxis.end())
			return null_axis;

		return axisIt->second;
	}

	Action& InputMapper::FindAction(std::string_view actionName)
	{
		static Action null_action{ .Name = "NULL", .ID = 0 };

		auto it = actionNames.find(std::string(actionName));

		if (it == actionNames.end())
			return null_action;

		const unsigned long long id = it->second;

		auto actionIt = registeredActions.find(id);

		if (actionIt == registeredActions.end())
			return null_action;

		return actionIt->second;
	}

	bool InputMapper::IsActionPressed(const Action& action, ActionState state)
	{
		for (int i = 0; i < action.Keys.size(); i++)
		{
			switch (state)
			{
			case Eclipse::Input::ActionState::WasPressed:
			{
				if (Input::GetKeyDown(action.Keys[i]))
				{
					return true;
				}
			} break;

			case Eclipse::Input::ActionState::IsPressed:
			{
				if (Input::GetKey(action.Keys[i]))
				{
					return true;
				}
			} break;

			case Eclipse::Input::ActionState::WasReleased:
			{
				if (Input::GetKeyUp(action.Keys[i]))
				{
					return true;
				}
			} break;
			}
		}

		return false;
	}


	std::unordered_map<std::string, unsigned long long>& InputMapper::GetActionNames()
	{
		return actionNames;
	}

	std::unordered_map<std::string, unsigned long long>& Eclipse::Input::InputMapper::GetAxisNames()
	{
		return axisNames;
	}

	std::unordered_map<unsigned long long, Action>& Eclipse::Input::InputMapper::GetActions()
	{
		return registeredActions;
	}

	std::unordered_map<unsigned long long, Axis>& Eclipse::Input::InputMapper::GetAxises()
	{
		return registeredAxis;
	}
}