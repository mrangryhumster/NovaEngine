#ifndef COPENGLRENDERER_H
#define COPENGLRENDERER_H

#include "IWindow.h"
#include "IRenderer.h"
#include "SEngineConf.h"
#include "ERendererEnums.h"
#include "renderer\CRendererPerformanceCounter.h"
#include "log\NLogger.h"


#include "GL\glew.h"
#include <GL\gl.h>
#include <map>

#include "CBaseRenderer.h"
#include "opengl/COpenGLShaderProgram.h"
#include "opengl/COpenGLTexture.h"
#include "opengl/COpenGLVertexBuffer.h"

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
    RCSL_VERTEX_ARRAY  = 0,
    RCSL_TEXCORD_ARRAY = 1,
    RCSL_NORMAL_ARRAY  = 2,
    RCSL_COLOR_ARRAY   = 3,
    RCSL_STATES_COUNT  = 4,
};
//-------------------------

class COpenGLRenderer : public CBaseRenderer
{
public:
    COpenGLRenderer(window::IWindow* Window,SEngineConf conf);
    virtual ~COpenGLRenderer();

    u32 getType();
    //--------------------------------------------------------------------------
    void setVSync(bool);
    //--------------------------------------------------------------------------
    void setRenderState(u32 flag,URenderStateValue value);
    URenderStateValue getRenderState(u32 flag);
    //--------------------------------------------------------------------------
    s32  QueryRendererFeature(E_RENDERER_FEATURE feature);
    //--------------------------------------------------------------------------
    void setViewport(core::rectu);
    core::rectu getViewport();
    //--------------------------------------------------------------------------
    void setTransform(const core::matrixf& mat,E_MATRIX_TYPE mtype);
    const core::matrixf getTransform(E_MATRIX_TYPE mtype);
    void resetTransform(E_MATRIX_TYPE mtype);
    //--------------------------------------------------------------------------
    IShaderProgram* GenShaderProgram();
    //--------------------------------------------------------------------------
    IVertexBuffer* GenVertexBuffer();
    //--------------------------------------------------------------------------
    ITexture* GenTexture(IImage*,STextureParameters);
    IImage*   GenImage(ITexture*);
    //--------------------------------------------------------------------------
    void bindTexture(ITexture*,u32);
    void bindShaderProgram(IShaderProgram*);
    //--------------------------------------------------------------------------
    void bindMaterial(IMaterial*);
    //--------------------------------------------------------------------------
    void setRenderTarget(ITexture* target,u32 target_type);
    //--------------------------------------------------------------------------
    void clear(u32 flag,core::color4f clear_color = core::color4f(0,0,0,1));
    void flush();


    ///Render   HL
    void drawVertexBuffer(IVertexBuffer*);

    void drawPrimitiveList(const SVertex*,u32 VertexCount,E_PRIMITIVE_TYPE,u32 VertexFormat);
    void drawIndexedPrimitiveList(const u16* Index,u16 IndexCount,const SVertex*,u32 VertexCount,E_PRIMITIVE_TYPE,u32 VertexFormat);
    void drawArrays(u16 indices_count,u32 vertex_count,const u16* indices,const core::vector3f* verticles,const core::vector2f* texverts,const core::vector3f* normals,const core::color4u* colors,E_PRIMITIVE_TYPE);

    bool isOk();
    bool update();


protected:
private:
    inline void to_opengl_primitive(E_PRIMITIVE_TYPE engine_primitive,u32& gl_primitive,u32& vertexperprimitive);
    inline u32  to_opengl_blendmodes(E_BLENDING_MODE engine_mode);
    inline void enable_client_states(bool vert,bool tex,bool norm,bool color);

    bool RendererClientStatesList[RCSL_STATES_COUNT];


    //------------------------RTT
    u32 active_framebuffer_object;
    u32 active_renderbuffer_object;
    //---------------------------

    #ifdef NE_WINDOW_WIN32
    HWND  hWnd;
    HDC   hDC;
    HGLRC hRC;
    #endif // NE_WINDOW_WIN32
};

}
}
#endif // COPENGLRENDERER_H
