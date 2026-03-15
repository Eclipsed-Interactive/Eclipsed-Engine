#pragma once

#include <vector>
#include <functional>

#include "CoreEngine/ErrorCodes.h"

#include "CoreEngine/Math/Math.h"
#include "CoreEngine/Math/Color.h"

#include "UniformVariableManager.h"
#include "GPUInfo.h"

#undef CreateWindow

struct GLFWwindow;
struct GLFWcursor;

namespace Eclipse
{
	class GraphicsEngine
	{
	public:
		static ErrorCode Init();
		static void BeginFrame();
		static void Render();
		static void EndFrame();

		static int ShouldWindowClose();

		static ErrorCode CheckErrorCodes(ErrorCode aErrorCode);

		static void AddGlobalUniform(UniformType aType, const char* aUniformName, void* aValue);
		static void UpdateGlobalUniform(UniformType aType, const char* aUniformName, void* aValue);
		static void GetGlobalUniform(UniformType aType, const char* aUniformName, void* aValue);

		static void SetUniform(UniformType aType, unsigned aShaderProgram, const char* aUniformName, void* aValue);

		static void SetGlobalUniforms(unsigned aShaderProgram);

		static void BindTexture(int aGLType, unsigned aTextureID);

		static void BindFrameBuffer(unsigned aFrameBuffer);

		static void ClearCurrentSceneBuffer(
			float aClearColorR = myClearColor.r,
			float aClearColorG = myClearColor.g,
			float aClearColorB = myClearColor.b,
			float aClearColorA = 1.f);

		static void RegisterListenToResolutionChange(const std::function<void()>& aLambda);

		static Math::Vector2i GetWindowPosition();

		static Math::Vector4ui ReadPixel(const Math::Vector2ui& aPos);

		static void SetWindowIcon(const char* aPath);

		static void CreateOpenGLTexture(unsigned& textureID, Math::Vector2f& spriteDivOne, float& dimDivOne, int channels, int width, int height, unsigned char* aPixels);

	private:
		static inline GLFWwindow* myWindow;
		static inline UniformVariableManager myUniformManager;
		static inline Math::Color myClearColor;

	private:
		static ErrorCode InitOpenGL();
		static ErrorCode CreateWindow();

	public:
		enum class MouseCursor {
			Hand,
			Grab
		};

		static void SetCursor(MouseCursor aMouseCursor);
		static void ResetCursor();
	private:

		static inline std::vector<GLFWcursor*> myMouseCursors;
	};
}
