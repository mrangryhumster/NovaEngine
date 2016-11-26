#pragma once

#include "IRenderTarget.h"
#include "IRenderer.h"

#include "NLogger.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include "COpenGLTexture.h" //not sure if need this

namespace novaengine
{
	namespace renderer
	{
		class COpenGLRenderTarget : public IRenderTarget
		{
		public:
			COpenGLRenderTarget(IRenderer* p_OGLRenderer);
			virtual ~COpenGLRenderTarget();

			void setTexture(renderer::ITexture* Target, u32 AttachTo);

			bool isOk();

			GLuint& getFramebuffer();

		protected:
		private:

			IRenderer* m_OGLRenderer;
			bool m_Framebuffer_ContructFailed;

			GLuint m_GLFramebufferID;
			ITexture* m_AttachedTextures[ERTT_TARGET_COUNT];
		};

	}
}

