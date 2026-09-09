//#include "AbstractView.h"
//
//#include "ViewRegistry.h"
//
//namespace Eclipse::Editor
//{
//	AbstractView::AbstractView(const char* _name, const char* _category, bool _isOpen, ImGuiWindowFlags _flags)
//		: flags(_flags), isOpen(_isOpen), name(_name), category(_category)
//	{
//		id = rand();
//	}
//
//	const char* AbstractView::GetName() const
//	{
//		return name;
//	}
//
//	ImGuiWindowFlags AbstractView::GetFlags() const
//	{
//		return flags;
//	}
//	
//	bool* AbstractView::GetIsOpenPtr()
//	{
//		return &isOpen;
//	}
//	
//	bool AbstractView::GetIsOpen() const
//	{
//		return isOpen;
//	}
//
//	int AbstractView::GetID() const
//	{
//		return id;
//	}
//}