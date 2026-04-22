#include "WindowData.h"

#define s(T) #T

rapidjson::Value Eclipse::Editor::WindowData::Serialize(const WindowData& aVec, rapidjson::Document::AllocatorType& alloc)
{
    rapidjson::Value val(rapidjson::kObjectType);
    val.AddMember("type", s(Editor::WindowData), alloc);
    val.AddMember("id", aVec.id, alloc);
    val.AddMember("name", rapidjson::Value(aVec.name.c_str(), alloc).Move(), alloc);

    return val;
}

Eclipse::Editor::WindowData Eclipse::Editor::WindowData::Deserialize(const rapidjson::Value& aValue)
{
    Eclipse::Editor::WindowData data;

    data.id = aValue["id"].GetInt();
    data.name= aValue["name"].GetString();

    return data;
}
