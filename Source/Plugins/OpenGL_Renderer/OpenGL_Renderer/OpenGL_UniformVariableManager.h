#pragma once

#include "Renderer/UniformVariableManager.h"

namespace Eclipse::Graphics::OpenGL
{
	class OpenGL_UniformVariableManager : public UniformVariableManager
	{
	public:
        void SetGlobalUniforms(unsigned aShaderProgram) override;

        void SetUniformInt(const char* aUniformName, unsigned aShaderProgram, const int* aValue) const override;

        void SetUniformFloat(const char* aUniformName, unsigned aShaderProgram, const float* aValue) const override;

        void SetUniformVec2Float(const char* aUniformName, unsigned aShaderProgram, const Math::Vector2f* aValue) const override;
        void SetUniformVec3Float(const char* aUniformName, unsigned aShaderProgram, const Math::Vector3f* aValue) const override;
        void SetUniformVec4Float(const char* aUniformName, unsigned aShaderProgram, const Math::Vector4f* aValue) const override;

        void SetUniformMat2x2(const char* aUniformName, unsigned aShaderProgram, const float* aValue) const override;
        void SetUniformMat3x3(const char* aUniformName, unsigned aShaderProgram, const float* aValue) const override;
        void SetUniformMat4x4(const char* aUniformName, unsigned aShaderProgram, const float* aValue) const override;
	};
}