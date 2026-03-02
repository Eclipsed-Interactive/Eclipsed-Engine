#include "OpenGLGraphicsAPI.h"

#include <stdio.h>
#include <string>

#include "CoreEngine/Math/Vector/Vector2.h"

#include "GraphicsEngine/TextManager.h"
#include "DebugDrawers/DebugDrawer.h"
#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "stb_image/stb_image.h"

#include "CoreEngine/PathManager.h"

#include "OpenGL/GLFW/glfw3.h"
#include "OpenGL/glad/glad.h"

#include"CoreEngine/MainSingleton.h"
#include "CoreEngine/Settings/GraphicsSettings.h"
#include "CoreEngine/Settings/EngineSettings.h"

#include "CoreEngine/Debug/DebugLogger.h"

#undef CreateWindow

namespace Eclipse
{
	static std::vector<std::function<void()>> resolutionChangeCallbackFunctions;

	void SetWindowDimenstion()
	{
		const Math::Vector2i& resolution = Settings::GraphicsSettings::GetResolution();

		Math::Vector2f oneDivRes = { 1.f / resolution.x, 1.f / resolution.y };

		GraphicsEngine::UpdateGlobalUniform(UniformType::Vector2f, "resolutionMultiplier", &oneDivRes);

		float resolutionRatio = resolution.y / resolution.x;
		GraphicsEngine::UpdateGlobalUniform(UniformType::Float, "resolutionRatio", &resolutionRatio);

		for (auto& callBackFunc : resolutionChangeCallbackFunctions)
			callBackFunc();
	}

	void WindowChangeDimenstions(GLFWwindow* window, int width, int height)
	{
		if (!width || !height)
			return;


		glViewport(0, 0, width, height);

		Settings::GraphicsSettings::SetResolution(Math::Vector2i(width, height));
		SetWindowDimenstion();
	}

	void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	ErrorCode GraphicsEngine::CreateWindow()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		Math::Vector2i resolution = Settings::GraphicsSettings::GetResolution();
		float width = resolution.x;
		float height = resolution.y;

		std::string engineName = Settings::EngineSettings::GetEngineName();
		std::string engineVersion = Settings::EngineSettings::GetEngineVersion();

		std::string title = engineName + " " + engineVersion;

		myWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		int x, y;
		glfwGetWindowSize(myWindow, &x, &y);
		if (myWindow && x == 0 && y == 0)
		{
			return ErrorCode::FAILED;
		}


		SetWindowDimenstion();

		if (!myWindow)
			return ErrorCode::GLFW_WINDOW_FAILED_TO_CREATE;

		glfwMakeContextCurrent(myWindow);

		MainSingleton::RegisterInstance<GLFWwindow*>() = myWindow;

		return ErrorCode::SUCCESS;
	}

	ErrorCode GraphicsEngine::InitOpenGL()
	{
		glfwSetErrorCallback(error_callback);

		int iResult;

		// GLFW initialization
		iResult = glfwInit();
		if (!iResult)
			return ErrorCode::GLFW_FAILED_TO_INITILIZE;

		ErrorCode errorCode = CreateWindow();
		if (errorCode != ErrorCode::SUCCESS)
			return errorCode;

		// Glad initialization
		{
			iResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			if (!iResult)
				return ErrorCode::GLAD_FAILED_TO_INITILIZE;
		}

		// Setup Window size change callback
		glfwSetFramebufferSizeCallback(myWindow, WindowChangeDimenstions);

		// Extra GLFW settings
		glfwSwapInterval(0);

		unsigned int texId = 0;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		return ErrorCode::SUCCESS;
	}

	void EnableOpenGLSettings()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	ErrorCode GraphicsEngine::Init()
	{
		//nvmlInit();

		ErrorCode errorCode = InitOpenGL();
		EnableOpenGLSettings();
		DebugDrawer::Get().Init();
		TextManager::Get().Init();

		myClearColor.r = 0.4314f;
		myClearColor.g = 0.1804f;
		myClearColor.b = 0.6f;
		myClearColor.a = 1.0f;

		myMouseCursors.emplace_back(glfwCreateStandardCursor(GLFW_HAND_CURSOR));

		GLFWimage cursor;

		int nrChannels = 0;
		cursor.pixels = stbi_load((PathManager::GetEngineAssetsPath() / "GrabbyHand.png").generic_string().c_str(), &cursor.width, &cursor.height, &nrChannels, 0);
		myMouseCursors.emplace_back(glfwCreateCursor(&cursor, 8.f, 8.f));

		CommandListManager::InitAllCommandLists();

		return errorCode;
	}

	void GraphicsEngine::CreateOpenGLTexture(unsigned& textureID, Math::Vector2f& spriteDivOne, float& dimDivOne, int channels, int width, int height, unsigned char* aPixels)
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x2901);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x2901);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 0x2601);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 0x2601);

		int rgbTypeOffset = 3 - channels;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB - rgbTypeOffset, width, height, 0, GL_RGB - rgbTypeOffset, GL_UNSIGNED_BYTE, aPixels);
		glGenerateMipmap(GL_TEXTURE_2D);


		spriteDivOne = { 1.0f / static_cast<float>(width), 1.0f / static_cast<float>(height) };
		dimDivOne = 1.f / (static_cast<float>(height) / static_cast<float>(width));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GraphicsEngine::BeginFrame()
	{
		CommandListManager::GetHappenAtBeginCommandList().Execute();
		CommandListManager::GetHappenAtBeginCommandList().Reset();

		glfwMakeContextCurrent(myWindow);
		glfwPollEvents();

		DebugDrawer::Get().Begin();


		GraphicsEngine::BindFrameBuffer(0);
#ifdef ECLIPSEDEDITOR
		GraphicsEngine::ClearCurrentSceneBuffer(0.3f, 0.3f, 0.3f, 1);
#else
		GraphicsEngine::ClearCurrentSceneBuffer();
#endif
	}

	void GraphicsEngine::SetGlobalUniforms(unsigned aShaderProgram)
	{
		myUniformManager.SetGlobalUniforms(aShaderProgram);
	}

	void GraphicsEngine::Render()
	{
#ifndef ECLIPSED_EDITOR
		Math::Vector2i resolution = Settings::GraphicsSettings::GetResolution();

		glViewport(0, 0, resolution.x, resolution.y);
		

		float aspectRatio = static_cast<float>(resolution.y) / static_cast<float>(resolution.x);
		GraphicsEngine::UpdateGlobalUniform(UniformType::Float, "resolutionRatio", &aspectRatio);

		int ovColor = 1;
		GraphicsEngine::UpdateGlobalUniform(UniformType::Int, "notOverrideColor", &ovColor);
#endif

		CommandListManager::ExecuteAllCommandLists();

#ifdef ECLIPSED_EDITOR
		DebugDrawer::Get().Render();
#endif
	}
	void GraphicsEngine::EndFrame()
	{
		glfwSwapBuffers(myWindow);

		CommandListManager::ResetAllCommandLists();
	}

	int GraphicsEngine::ShouldWindowClose()
	{
		return glfwWindowShouldClose(MainSingleton::GetInstance<GLFWwindow*>());
	}

	ErrorCode GraphicsEngine::CheckErrorCodes(ErrorCode aErrorCode)
	{
		switch (aErrorCode)
		{
		case ErrorCode::GLFW_FAILED_TO_INITILIZE:
		{
			LOG_ERROR("GLFW failed to initilize");
			glfwTerminate();
			return ErrorCode::FAILED;
		}
		break;
		case ErrorCode::GLFW_WINDOW_FAILED_TO_CREATE:
		{
			LOG_ERROR("GLFW window was not able to be created");
			glfwTerminate();
			return ErrorCode::FAILED;
		}
		break;
		case ErrorCode::GLAD_FAILED_TO_INITILIZE:
		{
			LOG_ERROR("GLAD failed to initilize");
			glfwTerminate();
			return ErrorCode::FAILED;
		}
		break;
		}

		return ErrorCode::SUCCESS;
	}

	void GraphicsEngine::AddGlobalUniform(UniformType aType, const std::string& aUniformName, void* aValue)
	{
		switch (aType)
		{
		case UniformType::Bool: myUniformManager.AddInt(aUniformName, (int*)aValue); return;

		case UniformType::Float: myUniformManager.AddFloat(aUniformName, (float*)aValue); return;

		case UniformType::Vector2f: myUniformManager.AddVec2Float(aUniformName, (Math::Vector2f*)aValue); return;
		case UniformType::Vector3f: myUniformManager.AddVec3Float(aUniformName, (Math::Vector4f*)aValue); return;
		case UniformType::Vector4f: myUniformManager.AddVec4Float(aUniformName, (Math::Vector4f*)aValue); return;

		case UniformType::Int: myUniformManager.AddInt(aUniformName, (int*)aValue); return;

		case UniformType::Matrix2x2f: myUniformManager.AddMat2x2(aUniformName, (float*)aValue); return;
		case UniformType::Matrix3x3f: myUniformManager.AddMat3x3(aUniformName, (float*)aValue); return;
		case UniformType::Matrix4x4f: myUniformManager.AddMat4x4(aUniformName, (float*)aValue); return;
		}
	}

	void GraphicsEngine::UpdateGlobalUniform(UniformType aType, const std::string& aUniformName, void* aValue)
	{
		switch (aType)
		{
		case UniformType::Bool: myUniformManager.UpdateInt(aUniformName, (int*)aValue); return;

		case UniformType::Float: myUniformManager.UpdateFloat(aUniformName, (float*)aValue); return;

		case UniformType::Vector2f: myUniformManager.UpdateVec2Float(aUniformName, (Math::Vector2f*)aValue); return;
		case UniformType::Vector3f: myUniformManager.UpdateVec3Float(aUniformName, (Math::Vector4f*)aValue); return;
		case UniformType::Vector4f: myUniformManager.UpdateVec4Float(aUniformName, (Math::Vector4f*)aValue); return;

		case UniformType::Int: myUniformManager.UpdateInt(aUniformName, (int*)aValue); return;

		case UniformType::Matrix2x2f: myUniformManager.UpdateMat2x2(aUniformName, (float*)aValue); return;
		case UniformType::Matrix3x3f: myUniformManager.UpdateMat3x3(aUniformName, (float*)aValue); return;
		case UniformType::Matrix4x4f: myUniformManager.UpdateMat4x4(aUniformName, (float*)aValue); return;
		}
	}

	void GraphicsEngine::GetGlobalUniform(UniformType aType, const std::string& aUniformName, void* aValue)
	{
		switch (aType)
		{
		case UniformType::Bool: myUniformManager.GetInt(aUniformName, (int*)aValue); return;

		case UniformType::Float: myUniformManager.GetFloat(aUniformName, (float*)aValue); return;

		case UniformType::Vector2f: myUniformManager.GetVec2Float(aUniformName, (Math::Vector2f*)aValue); return;
		case UniformType::Vector3f: myUniformManager.GetVec3Float(aUniformName, (Math::Vector4f*)aValue); return;
		case UniformType::Vector4f: myUniformManager.GetVec4Float(aUniformName, (Math::Vector4f*)aValue); return;

		case UniformType::Int: myUniformManager.GetInt(aUniformName, (int*)aValue); return;

		case UniformType::Matrix2x2f: myUniformManager.GetMat2x2(aUniformName, (float*)aValue); return;
		case UniformType::Matrix3x3f: myUniformManager.GetMat3x3(aUniformName, (float*)aValue); return;
		case UniformType::Matrix4x4f: myUniformManager.GetMat4x4(aUniformName, (float*)aValue); return;
		}
	}

	void GraphicsEngine::SetUniform(UniformType aType, unsigned aShaderProgram, const std::string& aUniformName, void* aValue)
	{
		switch (aType)
		{
		case UniformType::Bool: myUniformManager.SetUniformInt(aUniformName, aShaderProgram, (int*)aValue); return;

		case UniformType::Float: myUniformManager.SetUniformFloat(aUniformName, aShaderProgram, (float*)aValue); return;

		case UniformType::Vector2f: myUniformManager.SetUniformVec2Float(aUniformName, aShaderProgram, (Math::Vector2f*)aValue); return;
		case UniformType::Vector3f: myUniformManager.SetUniformVec3Float(aUniformName, aShaderProgram, (Math::Vector4f*)aValue); return;
		case UniformType::Vector4f: myUniformManager.SetUniformVec4Float(aUniformName, aShaderProgram, (Math::Vector4f*)aValue); return;

		case UniformType::Int: myUniformManager.SetUniformInt(aUniformName, aShaderProgram, (int*)aValue); return;

		case UniformType::Matrix2x2f: myUniformManager.SetUniformMat2x2(aUniformName, aShaderProgram, (float*)aValue); return;
		case UniformType::Matrix3x3f: myUniformManager.SetUniformMat3x3(aUniformName, aShaderProgram, (float*)aValue); return;
		case UniformType::Matrix4x4f: myUniformManager.SetUniformMat4x4(aUniformName, aShaderProgram, (float*)aValue); return;
		}
	}

	void GraphicsEngine::BindTexture(int aGLType, unsigned aTextureID)
	{
		glBindTexture(aGLType, aTextureID);
	}

	void GraphicsEngine::BindFrameBuffer(unsigned aFrameBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, aFrameBuffer);
	}

	void GraphicsEngine::ClearCurrentSceneBuffer(float aClearColorR, float aClearColorG, float aClearColorB, float aClearColorA)
	{
		glClearColor(aClearColorR, aClearColorG, aClearColorB, aClearColorA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GraphicsEngine::RegisterListenToResolutionChange(const std::function<void()>& aLambda)
	{
		resolutionChangeCallbackFunctions.emplace_back(aLambda);
	}
	Math::Vector2i GraphicsEngine::GetWindowPosition()
	{
		int xpos, ypos;
		glfwGetWindowPos(myWindow, &xpos, &ypos);
		return Math::Vector2i(xpos, ypos);
	}


	void GraphicsEngine::SetCursor(MouseCursor aCursor)
	{
		GLFWcursor* curs = myMouseCursors[static_cast<int>(aCursor)];

		glfwSetCursor(myWindow, curs);
	}

	void GraphicsEngine::ResetCursor()
	{
		glfwSetCursor(myWindow, nullptr);
	}

	Math::Vector4ui GraphicsEngine::ReadPixel(const Math::Vector2ui& aPos)
	{
		glFlush();
		glFinish();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char data[4];
		glReadPixels(aPos.x, aPos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		return Math::Vector4ui(data[0], data[1], data[2], data[3]);
	}

	void GraphicsEngine::SetWindowIcon(const char* aPath)
	{
		GLFWimage images[1];
		images[0].pixels = stbi_load(aPath, &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(MainSingleton::GetInstance<GLFWwindow*>(), 1, images);
		stbi_image_free(images[0].pixels);
	}
}

