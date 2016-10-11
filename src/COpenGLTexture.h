#pragma once

#include "NovaEngine.h"

#include "ne_types.h"
#include "ITexture.h"
#include "STextureParameters.h"

#include "COpenGLRenderer.h"
#include "CImage.h"

#include <GL/gl.h>

namespace novaengine
{
	namespace renderer
	{

		class COpenGLTexture : public ITexture
		{
		public:
			COpenGLTexture(IRenderer* OGLRenderer, IImage* Image, STextureParameters TextureParameters);
			virtual ~COpenGLTexture();

			void setTextureParameter(u32 Parameter, u32 value);
			u32  getTextureParameter(u32 Parameter);
			core::dim2u getTextureDimension();

			void bind();
			IImage* lock();
			void unlock();

			GLuint getTextureID();

		protected:
		private:
			IRenderer* OpenGLRenderer;

			STextureParameters  TextureParameters;
			core::dim2u         TextureDimension;
			GLuint              GLTexture;

			IImage* TextureImg; //!< Fills by using lock(), will be deleted after call unlock().
		};

	}
}
