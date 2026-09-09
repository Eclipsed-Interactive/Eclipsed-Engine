//#include "OpenGL_TextureData.h"
//
//namespace Eclipse::Graphics::OpenGL::Assets
//{
//	void OpenGL_TextureData::Bind(unsigned slot)
//	{
//		boundSlot = slot;
//
//		glActiveTexture(GL_TEXTURE0 + slot);
//		glBindTexture(GL_TEXTURE_2D, textureID);
//	}
//
//	void OpenGL_TextureData::Unbind(unsigned slot)
//	{
//		if (slot == 0)
//			boundSlot = 0;
//
//		glActiveTexture(GL_TEXTURE0 + boundSlot);
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//}