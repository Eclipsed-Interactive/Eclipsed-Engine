#pragma once

#include <string>
#include <functional>

namespace Eclipse::Editor
{
	class EditorViewInstance;
	struct EditorViewType
	{ 
		size_t id = 0;
		std::string name;

		std::function<EditorViewInstance* (int id)> create;
	};
}