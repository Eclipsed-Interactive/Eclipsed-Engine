#include "PixelShader.h"

namespace Eclipse
{
	unsigned PixelShader::GetProgramID() 
	{
		return data->shaderProgramID;
	}

	void PixelShader::Bind()
	{
		glUseProgram(data->shaderProgramID);
	}
}