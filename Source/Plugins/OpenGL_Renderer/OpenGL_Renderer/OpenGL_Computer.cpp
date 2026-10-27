#include "OpenGL_Computer.h"

#include "OpenGL/glad/glad.h"

#include "OpenGL_GraphicsDevice.h"
#include "Core/MainSingleton.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"

#include "Renderer/ParticleDrawer.h"

namespace Eclipse::Graphics::OpenGL
{
	void OpenGL_Computer::ComputeParticleUpdate(ParticleData* aParticles, int aParticleCount, unsigned SSBOID)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
		Graphics::IDrawer* drawer = renderer.GetDrawer();
		
		glEnable(GL_RASTERIZER_DISCARD);

		glUseProgram(drawer->myParticleDrawer->GetComputeProgramID());

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, aParticleCount * sizeof(ParticleData), aParticles, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOID);
		glDispatchCompute((aParticleCount + 63) / 64, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOID);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, aParticleCount * sizeof(ParticleData), aParticles);

		glDisable(GL_RASTERIZER_DISCARD);
	}

	unsigned OpenGL_Computer::CreateNewShaderStorageBufferObject(ParticleData* aParticles, int aParticleCount)
	{
		unsigned SSBOID = 0;

		glGenBuffers(1, &SSBOID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, aParticleCount * sizeof(ParticleData), aParticles, GL_DYNAMIC_DRAW);

		return SSBOID;
	}
}