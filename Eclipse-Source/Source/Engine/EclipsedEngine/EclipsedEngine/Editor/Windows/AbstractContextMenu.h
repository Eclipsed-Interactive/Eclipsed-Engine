#ifdef ECLIPSED_EDITOR
#pragma once

namespace Eclipse::Editor
{
	class AbstractContextMenu {
	public:
		AbstractContextMenu(const char* name);

		void Draw();

		virtual void Update() = 0;
		virtual void UpdateAlways() {}

		bool BeginMenu() const ;
		void EndMenu() const ;

	private:
		const char* ctxName;
		bool showMenu = false;
	};
}
#endif