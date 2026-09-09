//#include "ViewManager.h"
//
//#include "AbstractView.h"
//#include "ViewRegistry.h"
//#include "ImGui/imgui.h"
//
//namespace Eclipse::Editor
//{
//	std::vector<AbstractView*> ViewManager::views;
//
//	void ViewManager::Init()
//	{
//		//auto view = ViewRegistry::GetView("AssetBrowserView");
//		//views.push_back((AbstractView*)view());
//	}
//
//	void ViewManager::Update()
//	{
//		for (int i = 0; i < (int)views.size(); i++)
//		{
//			AbstractView* view = views[i];
//
//			std::string title = view->GetName();
//			title += "##" + std::to_string(view->GetID());
//
//			ImGui::Begin(title.c_str(), view->GetIsOpenPtr(), view->GetFlags());
//			view->PreUpdate();
//			view->Update();
//			view->PostUpdate();
//			ImGui::End();
//		}
//
//	}
//}