#include <windows.h>
#include <filesystem>
#include <iostream>

#include <assert.h>

#include "OpenGL/glad/glad.h"
#include "OpenGL/GLFW/glfw3.h"


#define GLFW_EXPOSE_NATIVE_WIN32
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

#include "ProjectData.h"

#include "CoreEngine/Files/FileUtilities.h"

#pragma region OPENGL AND IMGUI | MAIN

Projects projects;
GLFWwindow* window;
bool shouldClose = false;

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void InitOpenGL()
{
	glfwSetErrorCallback(error_callback);

	int iResult;

	// GLFW initialization
	iResult = glfwInit();
	if (!iResult) assert("OpenGL failed to initialize.");


	{ // Create the window.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
		std::string engineName = "Eclipsed Hub";
		std::string engineVersion = "v0.0.0";

		std::string title = engineName + " | " + engineVersion;

		window = glfwCreateWindow(1024, 568, title.c_str(), nullptr, nullptr);

		int x, y;
		glfwGetWindowSize(window, &x, &y);
		if (window && x == 0 && y == 0)
		{
			assert(("OpenGL failed to create a window." + std::to_string(x) + "  |  " + std::to_string(y)).c_str());
		}


		if (!window) assert("OpenGL failed to create a window.");

		glfwMakeContextCurrent(window);
	}

	// Glad initialization
	{
		iResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!iResult) assert("GLAD failed to initialize.");
	}

	// Extra GLFW settings
	glfwSwapInterval(0);

	unsigned int texId = 0;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void BeginFrameOpenGL()
{
	glfwMakeContextCurrent(window);
	glfwPollEvents();
}

void EndFrameOpenGL()
{
	glfwSwapBuffers(window);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void BeginFrameImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(1, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void EndFrameImGui()
{
	ImGui::EndFrame();
}

void RenderImGui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void BeginMainWindow()
{
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("MAIN WINDOW", nullptr, ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse);
}

void EndMainWindow()
{
	ImGui::End();
}

bool ShouldClose() { return glfwWindowShouldClose(window) || shouldClose; }

void Update();
void InitHub();
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InitOpenGL();
	InitImGui();

	InitHub();

	while (!ShouldClose())
	{
		BeginFrameOpenGL();
		BeginFrameImGui();

		BeginMainWindow();
		Update();
		EndMainWindow();

		RenderImGui();

		EndFrameOpenGL();
		EndFrameImGui();
	}
}

#pragma endregion




void CreateNewProjectAtPath(const char* path)
{
	std::filesystem::create_directory(path);
	projects.projects.push_back({ path, "v0.0.0" });
}

void CreateNewProject()
{
	const std::string directoryPath = Eclipse::Files::SelectFolderDialog();
	CreateNewProjectAtPath(directoryPath.c_str());
}

void OpenProjectAtPath(const char* path)
{

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	std::string exePath = "\"Eclipsed Engine_Debug.exe\"";
	std::string args = std::string("\"") + path + "\"";
	std::string cmdLine = exePath + " " + args;

	if (CreateProcess(
		NULL,
		&cmdLine[0],
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi)) {

		glfwHideWindow(window);

		WaitForSingleObject(pi.hProcess, INFINITE);

		glfwShowWindow(window);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void OpenProject()
{
	const std::string directoryPath = Eclipse::Files::SelectFolderDialog();
	projects.projects.push_back({ directoryPath, "v0.0.0" });

	OpenProjectAtPath(directoryPath.c_str());
}





void InitHub()
{

}

void Update()
{
	if (ImGui::Button("New Project"))
	{
		CreateNewProject();
	}

	if (ImGui::Button("Open Project"))
	{
		CreateNewProject();
	}



	ImGui::SetCursorPosX(400);
	ImGui::BeginChild("Projects");
	for (int i = 0; i < projects.projects.size(); i++)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(25, 25, 25, 255));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
		ProjectData& projData = projects.projects[i];

		if (projData.isHovered) ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));
		ImGui::BeginChild(std::filesystem::path(projData.path).string().c_str(), ImVec2(400, 60), ImGuiChildFlags_Borders);
		if (projData.isHovered)ImGui::PopStyleColor();

		const std::string name = std::filesystem::path(projData.path).filename().string();

		const int nameWidth = ImGui::CalcTextSize(name.c_str()).x;
		ImGui::SetCursorPosX((400 - nameWidth) * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
		ImGui::Text(name.c_str());

		ImGui::SetNextItemWidth(300);
		ImGui::Separator();

		std::string path = projData.path;
		std::string truncatedPath = path;
		const std::string ellipsis = "...";

		bool wasChanged = false;
		while (truncatedPath.length() > ellipsis.length())
		{
			truncatedPath = path.substr(path.length() - (truncatedPath.length() - ellipsis.length() - 1));
			if (ImGui::CalcTextSize(truncatedPath.c_str()).x <= 400 - ImGui::CalcTextSize("...").x) 
			{
				if(wasChanged)path = ellipsis + truncatedPath.substr(4, truncatedPath.length());
				break;
			}

			wasChanged = true;
		}

		const int pathWidth = ImGui::CalcTextSize(path.c_str()).x;
		ImGui::SetCursorPosX((400 - pathWidth) * 0.5f);

		ImGui::Text(path.c_str());
		ImGui::EndChild();


		if (ImGui::IsItemHovered()) projData.isHovered = true;
		else projData.isHovered = false;

		if (ImGui::BeginPopup(projData.path.c_str()))
		{
			if (ImGui::Button("Delete"))
			{
				projects.projects.erase(projects.projects.begin() + i);
				ImGui::EndPopup();
				ImGui::PopStyleColor();
				break;
			}
			if (ImGui::Button("Show in explorer"))
			{
				ShellExecute(NULL, "open", "explorer", projData.path.c_str(), NULL, SW_SHOWNORMAL);
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked()) OpenProjectAtPath(projData.path.c_str());
		else if (ImGui::IsItemClicked(1)) ImGui::OpenPopup(projData.path.c_str());

		ImGui::PopStyleColor();
	}
	ImGui::EndChild();
}