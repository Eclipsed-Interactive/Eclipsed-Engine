#include "OpenGL_Computer.h"

#include "OpenGL/glad/glad.h"

#include "OpenGL_GraphicsDevice.h"
#include "Core/MainSingleton.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"

#include "Renderer/ParticleCommon.h"


namespace Eclipse::Graphics::OpenGL
{
	void OpenGL_Computer::ComputeParticleSpawning(unsigned ParticlesToSpawn)
	{
		Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
		Graphics::IDrawer* drawer = renderer.GetDrawer();
	
	
		glUseProgram(drawer->myParticleDrawer->GetSpawnProgramID());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, drawer->myParticleDrawer->GetParticleDataSSBOID());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, drawer->myParticleDrawer->GetFreeListSSBOID());
		glDispatchCompute((ParticlesToSpawn + 63) / 64, 1, 1);
	}

	void OpenGL_Computer::ComputeParticleUpdate()
	{
		Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
		Graphics::IDrawer* drawer = renderer.GetDrawer();

		//glEnable(GL_RASTERIZER_DISCARD);

		glUseProgram(drawer->myParticleDrawer->GetProgressProgramID());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, drawer->myParticleDrawer->GetParticleDataSSBOID());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, drawer->myParticleDrawer->GetFreeListSSBOID());
		glDispatchCompute((Particles::maxParticles + 63) / 64, 1, 1);

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		//glDisable(GL_RASTERIZER_DISCARD);
	}
}