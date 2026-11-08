#include "OpenGL_Drawer.h"

#include "Renderer/Sprite.h"
#include "Assets/Assets/MaterialAsset.h"

#include "OpenGL/glad/glad.h"
#include "OpenGL_GraphicsDevice.h"
#include "Renderer/RendererManager.h"
#include "Renderer/IRenderer.h"

#include "Renderer/ParticleCommon.h"

namespace Eclipse::Graphics::OpenGL
{
	void OpenGL_Drawer::DrawSprite()
	{
		glBindVertexArray(mySprite->GetSpriteBufferID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void OpenGL_Drawer::DrawParticles()
	{
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

		glUseProgram(myParticleDrawer->GetProgramID());

		unsigned albedo0 = glGetUniformLocation(myParticleDrawer->GetProgramID(), "material[0].albedo");
		for(int i = 0; i < 8; i++)
			glUniform1i(albedo0 + i, i);

		glBindVertexArray(myParticleDrawer->GetBufferID());
		glDrawArrays(GL_POINTS, 0, Particles::maxParticles);
		glBindVertexArray(0);
	}

	//void OpenGL_Drawer::DrawTextSprite()
	//{
	//	//glBindVertexArray(sprite.GetSpriteBufferID());
	//	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//	//glBindVertexArray(0);
	//}

	//void OpenGL_Drawer::DrawSprite(SpriteRenderer2D* spriteRend, Transform2D* transform)
	//{
	//	//Assets::Material* mat = spriteRend->GetMaterial();
	//	//Graphics::IRenderer& r = Graphics::RendererManager::GetRenderer();
	//	//r.GetDevice()->BindMaterial(*mat);


	//}
}