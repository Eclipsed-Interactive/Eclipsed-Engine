#pragma once

namespace Eclipse
{
	class InputMap final
	{
	public:
		static InputMap CreateEmptyMap();



	private:
		std::string name;
		std::unordered_map<std::string, InputAction> map;
	};
}