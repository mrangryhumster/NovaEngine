#include "COpenGLRenderTarget.h"


namespace novaengine
{
	namespace renderer
	{

		COpenGLRenderTarget::COpenGLRenderTarget(IRenderer* p_OGLRenderer):
			m_OGLRenderer(p_OGLRenderer),
			m_Framebuffer_ContructFailed(false),
			m_GLFramebufferID(0)
		{
			glGenFramebuffers(1, &m_GLFramebufferID);

			for (u32 i = 0; i < ERTT_TARGET_COUNT; i++)
				m_AttachedTextures[i] = nullptr;
		}

		COpenGLRenderTarget::~COpenGLRenderTarget()
		{
			for (u32 i = 0; i < ERTT_TARGET_COUNT; i++)
				if (m_AttachedTextures[i])
					m_AttachedTextures[i]->release();

			glDeleteFramebuffers(1, &m_GLFramebufferID);
		}

		void COpenGLRenderTarget::setTexture(renderer::ITexture * p_Target, u32 p_AttachTo)
		{
			if (p_Target != m_AttachedTextures[p_AttachTo])
			{
				if (m_AttachedTextures[p_AttachTo])
					m_AttachedTextures[p_AttachTo]->release();
				if (p_Target)
					p_Target->capture();
				m_AttachedTextures[p_AttachTo] = p_Target;
			}
			
			GLuint OGL_TextureID = 0;

			if(p_Target)
				OGL_TextureID = reinterpret_cast<COpenGLTexture*>(p_Target)->getTexture();

			glBindFramebuffer(GL_FRAMEBUFFER, m_GLFramebufferID);

			switch (p_AttachTo)
			{
			case ERTT_COLOR_BUFFER_0:
			case ERTT_COLOR_BUFFER_1:
			case ERTT_COLOR_BUFFER_2:
			case ERTT_COLOR_BUFFER_3:
			case ERTT_COLOR_BUFFER_4:
			case ERTT_COLOR_BUFFER_5:
			case ERTT_COLOR_BUFFER_6:
			case ERTT_COLOR_BUFFER_7:
			case ERTT_COLOR_BUFFER_8:
			case ERTT_COLOR_BUFFER_9:
			case ERTT_COLOR_BUFFER_10:
			case ERTT_COLOR_BUFFER_11:
			case ERTT_COLOR_BUFFER_12:
			case ERTT_COLOR_BUFFER_13:
			case ERTT_COLOR_BUFFER_14:
			case ERTT_COLOR_BUFFER_15:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + p_AttachTo, GL_TEXTURE_2D, OGL_TextureID, 0);
				break;
			case ERTT_DEPTH_BUFFER:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, OGL_TextureID, 0);
				break;
			default:
				LOG_ERROR("COpenGLRenderTarget : invalid attachment\n");
			}

			//update renderbuffers
			std::vector<GLenum> OGL_RenderBuffers_vector;
			for (u32 i = 0; i < ERTT_COLOR_BUFFER_LAST; i++)
				if (m_AttachedTextures[i])
					OGL_RenderBuffers_vector.push_back(GL_COLOR_ATTACHMENT0 + i);
			if(OGL_RenderBuffers_vector.size() > 0)
			glDrawBuffers(OGL_RenderBuffers_vector.size(),&OGL_RenderBuffers_vector.front());
			
		}

		bool COpenGLRenderTarget::isOk()
		{
			if (m_Framebuffer_ContructFailed)
				return false;
			//----------------------------------------------------------------------
			//FBO error check block
			GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (err != GL_FRAMEBUFFER_COMPLETE)
			{
				LOG_ERROR("Something wrong with framebuffer...\n");
				switch (err)
				{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					LOG_ENGINE_DEBUG("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
					LOG_ENGINE_DEBUG("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n");
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					LOG_ENGINE_DEBUG("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
					break;
				case GL_FRAMEBUFFER_UNSUPPORTED:
					LOG_ENGINE_DEBUG("GL_FRAMEBUFFER_UNSUPPORTED\n");
					break;
				default:
					LOG_ENGINE_DEBUG("Unknow error.\n");
				}
				return false;
			}
			//----------------------------------------------------------------------
			return true;
		}

		GLuint & COpenGLRenderTarget::getFramebuffer()
		{
			return m_GLFramebufferID;
		}

	}

}