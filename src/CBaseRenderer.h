#ifndef CBASERENDERER_H
#define CBASERENDERER_H

#include "IWindow.h"
#include "IRenderer.h"
#include "SEngineConf.h"
#include "ERendererEnums.h"

#include "NLogger.h"
#include "CPerformanceCounter.h"

#include <map>

namespace novaengine
{
namespace renderer
{

class CBaseRenderer : public IRenderer
{
public:
    CBaseRenderer(CPerformanceCounter* PerformanceCounter,window::IWindow* wnd,SEngineConf conf);
    virtual ~CBaseRenderer();

    u32 getType();
    //--------------------------------------------------------------------------
    void setVSync(bool);
    bool getVSync();
    //--------------------------------------------------------------------------
    void setRenderState(u32 flag,URenderStateValue value);
    URenderStateValue getRenderState(u32 flag);
    //--------------------------------------------------------------------------
    void setViewport(core::rectu);
    core::rectu getViewport();
    //--------------------------------------------------------------------------
    void setTransform(const core::matrixf& mat,E_MATRIX_TYPE mtype);
    const core::matrixf getTransform(E_MATRIX_TYPE mtype);
    //--------------------------------------------------------------------------
    void bindTexture(ITexture*,u32);
    void bindShaderProgram(IShaderProgram*);
    //--------------------------------------------------------------------------
	void setRenderTarget(ITexture*, u32);
	void setRenderTarget(IRenderTarget*);
    //--------------------------------------------------------------------------
    void begin_frame(bool clear_color_buffer,bool clear_zbuffer,core::color4f clear_color);
    void end_frame();
    //--------------------------------------------------------------------------

    ///Render   HL
    void drawPrimitiveList(const SVertex*,u32 VertexCount,E_PRIMITIVE_TYPE,u32 VertexFormat);

    void drawPoint(core::vector3f Position);
    void drawLine(core::vector3f Start,core::vector3f End);
    void drawTriangle(core::vector3f Pos1,core::vector3f Pos2,core::vector3f Pos3);
    void drawQuad(core::vector3f Pos1,core::vector3f Pos2,core::vector3f Pos3,core::vector3f Pos4);

    void drawScreenQuad(ITexture* Texture,core::color4f color);

	bool isReady();
    bool isOk();


protected:

	void ClearCache();

	u32  m_RendererLastError;
    bool m_RendererReady;
	bool m_Renderer_Exit; // ?!

    SEngineConf       m_EngineConf;
    window::IWindow*  m_EngineWindow;
    //------------------------
    const char* m_RendererVersionName;
    const char* m_RendererVendorName;
    const char* m_RendererName;
    //------------------------
    core::rectu m_ViewportRect;
    core::dim2u m_ViewportSize;
    core::dim2u m_FrameSize;
    //------------------------
    core::matrixf m_MVPMatrix;
    core::matrixf m_ProjectionMatrix;
    core::matrixf m_ViewMatrix;
    core::matrixf m_ModelMatrix;
    core::matrixf m_TextureMatrix;
    //------------------------
    CPerformanceCounter* m_PerformanceCounter;
    //cache
    ITexture*       m_ActiveTexture[ERTU_TEXTURE_COUNT];
    IShaderProgram* m_ActiveProgram;
	IRenderTarget*  m_ActiveRenderTarget;

	//Simple RTT
	IRenderTarget* m_RenderTarget;

    //Render States
    std::map<int,URenderStateValue> m_RenderStates;

	//VSync(no idea where to put this)
    bool m_VSync;

private:

};
}
}

#endif // CBASERENDERER_H
