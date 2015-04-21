#ifndef CBASERENDERER_H
#define CBASERENDERER_H

#include "IWindow.h"
#include "IRenderer.h"
#include "SEngineConf.h"
#include "ERendererEnums.h"

#include "NLog.h"

#include "renderer\CRendererPerformanceCounter.h"
#include "renderer\CVertexBuffer.h"

#include <map>
namespace novaengine
{
namespace renderer
{

class CBaseRenderer : public IRenderer
{
public:
    CBaseRenderer(window::IWindow* wnd,SEngineConf conf);
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
    IRendererPerformanceCounter* getRendererPerformanceCounter();
    //--------------------------------------------------------------------------
    void bindTexture(ITexture*,u32);
    void bindShaderProgram(IShaderProgram*);
    //--------------------------------------------------------------------------
    void bindMaterial(IMaterial*);
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


    bool isOk();


protected:
    bool ready;
    bool noerror;
    bool exit;

    SEngineConf                 EngConf;
    window::IWindow*            Window;

    //------------------------
    const char* versionname;
    const char* vendorname;
    const char* renderername;
    const char* extensionlist;
    //------------------------
    core::rectu ViewportRect;
    core::dim2u ViewportSize;
    core::dim2u FrameSize;
    //------------------------
    core::matrixf ProjectionMatrix;
    core::matrixf ViewMatrix;
    core::matrixf ModelMatrix;
    core::matrixf TextureMatrix;
    //------------------------
    CRendererPerformanceCounter* RendererPerformanceCounter;

    //cache
    ITexture*       ActiveTexture[EMTN_TEXTURE_COUNT];
    IShaderProgram* ActiveProgram;
    IMaterial*      ActiveMaterial;
    //Render States
    std::map<int,URenderStateValue> RenderStates;
    //VSync(RLY?0_0)
    bool VSync;

private:

};
}
}

#endif // CBASERENDERER_H
