#include "VertexShader.h"

namespace Eclipse
{
	unsigned VertexShader::GetProgramID() const
	{
		return data->shaderProgramID;
	}

	void VertexShader::Bind()
	{
		glUseProgram(data->shaderProgramID);
	}
}