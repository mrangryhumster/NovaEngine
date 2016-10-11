#pragma once

#include "IWindow.h"
#include "IRenderer.h"
#include "SEngineConf.h"
#include "ERendererEnums.h"

#include "NLogger.h"
#include "NTime.h"

#include "GL/glew.h"
#include <GL/gl.h>
#include <map>

#include "CBaseRenderer.h"
#include "COpenGLShaderProgram.h"
#include "COpenGLTexture.h"
#include "COpenGLMeshBuffer.h"
#include "COpenGLRenderTarget.h"

#include "CMaterial.h"
#include "COpenGLShaderProgramLibrary.h"

#include "IEventListener.h" //also renderer can listen events to process specific, like window resize

#ifdef NE_WINDOW_WIN32
#include <windows.h>
#include "wglext.h"
#endif // NE_WINDOW_WIN32


namespace novaengine
{
	namespace renderer
	{

		//-------------------------
		enum R_CLIENT_STATES_LIST
		{
			RCSL_VERTEX_ARRAY = 0,
			RCSL_TEXCORD_ARRAY = 1,
			RCSL_NORMAL_ARRAY = 2,
			RCSL_COLOR_ARRAY = 3,
			RCSL_STATES_COUNT = 4,
		};
		//-------------------------

		class COpenGLRenderer : public CBaseRenderer, IEventListener
		{
		public:
			COpenGLRenderer(CPerformanceCounter* PerformanceCounter, window::IWindow* Window, SEngineConf conf);
			virtual ~COpenGLRenderer();

			u32 getType();
			//--------------------------------------------------------------------------
			void OnEvent(SEvent& Event);
			//--------------------------------------------------------------------------
			void setVSync(bool);
			//--------------------------------------------------------------------------
			void setRenderState(u32 flag, URenderStateValue value);
			URenderStateValue getRenderState(u32 flag);
			//--------------------------------------------------------------------------
			s32  QueryRendererFeature(E_RENDERER_FEATURE feature);
			//--------------------------------------------------------------------------
			void setViewport(core::rectu);
			//--------------------------------------------------------------------------
			void setTransform(const core::matrixf& mat, E_MATRIX_TYPE mtype);
			//--------------------------------------------------------------------------
			IRenderTarget*  createRenderTarget();
			//--------------------------------------------------------------------------
			IShaderProgram* createShaderProgram();
			IShaderProgram* requestShaderProgram(u32 shader_features);
			//--------------------------------------------------------------------------
			IMeshBuffer*    createMeshBuffer();
			//--------------------------------------------------------------------------
			ITexture*       createTexture(IImage*, STextureParameters);
			//--------------------------------------------------------------------------
			void bindTexture(ITexture*, u32);
			void bindShaderProgram(IShaderProgram*);
			//--------------------------------------------------------------------------
			void setRenderTarget(IRenderTarget* p_RenderTarget);
			//--------------------------------------------------------------------------
			void clear(u32 flag, core::color4f clear_color = core::color4f(0, 0, 0, 1));
			void flush();


			///Render   HL
			void drawMeshBuffer(IMeshBuffer*);

			void drawPrimitiveList(const SVertex*, u32 VertexCount, E_PRIMITIVE_TYPE, u32 VertexFormat);
			void drawIndexedPrimitiveList(const u16* Index, u16 IndexCount, const SVertex*, u32 VertexCount, E_PRIMITIVE_TYPE, u32 VertexFormat);
			void drawArrays(u16 indices_count, u32 vertex_count, const u16* indices, const core::vector3f* verticles, const core::vector2f* texverts, const core::vector3f* normals, const core::color4f* colors, E_PRIMITIVE_TYPE);


			bool isOk();
			bool update();

			void setShaderConstants();

		protected:
		private:

			COpenGLShaderProgramLibrary* m_ShaderProgramsLibrary;


			inline u32  to_opengl_blendmodes(E_BLENDING_MODE engine_mode);
			inline void enable_texture_unit(u32 p_TextureUnit);

			bool m_RendererActiveTextureUnitsList[ERTU_TEXTURE_COUNT];



#ifdef NE_WINDOW_WIN32
			HWND  hWnd;
			HDC   hDC;
			HGLRC hRC;
#endif // NE_WINDOW_WIN32
		};

	}
}
