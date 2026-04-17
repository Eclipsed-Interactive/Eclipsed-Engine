#include "pch.h"

#include "InputMapper.h"

#include "CoreEngine/Input/Keycodes.h"

#include <string>
#include <functional>

#include "Input.h"

#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"

//#include "AssetEngine/PathManager.h"

namespace Eclipse
{
	void InputMapper::Save()
	{
		using namespace rapidjson;

		Document d;
		d.SetObject();
		Document::AllocatorType& allocator = d.GetAllocator();

		Value inputList(kArrayType);

		for (auto& [name, inputAction] : map)
		{
			Value action = inputAction.Save(allocator);
			inputList.PushBack(action, allocator);
		}

		d.AddMember("inputActions", inputList, allocator);

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		d.Accept(writer);

		//std::ofstream ofs(PathManager::GetProjectRoot() / "Settings/InputMap.json");
		//ofs << buffer.GetString();
		//ofs.close();
	}

	void InputMapper::Load()
	{
		using namespace rapidjson;

		//std::ifstream ifs(PathManager::GetProjectRoot() / "Settings/InputMap.json");
		//if (!ifs.is_open()) {
		//	Save();
		//}

		//std::string jsonString((std::istreambuf_iterator<char>(ifs)),
		//	std::istreambuf_iterator<char>());

		//ifs.close();

		//Document d;
		//d.Parse(jsonString.c_str());

		//if (d.HasMember("inputActions"))
		//{
		//	const Value& inputList = d["inputActions"];
		//	for (auto& action : inputList.GetArray())
		//	{
		//		InputAction inputAction;
		//		inputAction.Load(action);

		//		map[inputAction.name] = inputAction;
		//	}
		//}
	}

	void InputMapper::Update()
	{
		for (auto& [name, action] : map)
		{
			action.Update();
		}
	}

	int InputMapper::ReadValue(const std::string& actionName)
	{
		if (map.find(actionName) == map.end())
			return 0;

		return map.at(actionName).value;
	}

	InputAction& InputMapper::AddAction(const std::string& actionName)
	{
		map[actionName] = InputAction(actionName);
		return map[actionName];
	}

}