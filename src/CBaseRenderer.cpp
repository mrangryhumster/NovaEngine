#include "CBaseRenderer.h"

#include "CMeshBuffer.h"

namespace novaengine
{
namespace renderer
{

CBaseRenderer::CBaseRenderer(CPerformanceCounter* pc,window::IWindow* wnd,SEngineConf conf):
    ready(false),
    noerror(true),
    exit(false),
    EngConf(conf),
    Window(wnd),
    versionname(nullptr),
    vendorname(nullptr),
    renderername(nullptr),
    PerformanceCounter(pc),
    ActiveProgram(nullptr),
    ActiveMaterial(nullptr),
	ActiveRenderTarget(nullptr),
	RenderTarget(nullptr)
{

    for(u32 i = 0; i < EMTN_TEXTURE_COUNT; i++)
        ActiveTexture[i] = nullptr;

}

CBaseRenderer::~CBaseRenderer()
{

}

//-----------------------------------------------------------------------------------------------
u32 CBaseRenderer::getType()
{
    return ERT_NULL;
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setVSync(bool flag)
{
    VSync = flag;
}
//-----------------------------------------------------------------------------------------------
bool CBaseRenderer::getVSync()
{
    return VSync;
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setRenderState(u32 flag,URenderStateValue value)
{
    if(flag < ERS_LAST_STATE)
        RenderStates[flag] = value;
}
//-----------------------------------------------------------------------------------------------
URenderStateValue CBaseRenderer::getRenderState(u32 flag)
{
    if(flag < ERS_LAST_STATE)
        return RenderStates[flag];
    return 0;
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setViewport(core::rectu nvp)
{
    ViewportRect = nvp;
    ViewportSize = core::dim2u(nvp.X2 - nvp.X1,nvp.Y2 - nvp.Y1);
}
//-----------------------------------------------------------------------------------------------
core::rectu CBaseRenderer::getViewport()
{
    return ViewportRect;
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setTransform(const core::matrixf& mat,E_MATRIX_TYPE mtype)
{

    switch(mtype)
    {
    case EMT_PROJECTION:
        //--------------------------------------
        ProjectionMatrix = mat;
        //--------------------------------------
        break;
    case EMT_VIEW:
    case EMT_MODEL:
        //--------------------------------------
        if(mtype == EMT_VIEW)
            ViewMatrix  = mat;
        else
            ModelMatrix = mat;
        //--------------------------------------
        break;
    case EMT_TEXTURE:
        //--------------------------------------
        TextureMatrix = mat;
        //--------------------------------------
        break;
    }
}
//-----------------------------------------------------------------------------------------------
const core::matrixf CBaseRenderer::getTransform(E_MATRIX_TYPE mtype)
{
    switch(mtype)
    {
    case EMT_PROJECTION:
        return ProjectionMatrix;
    case EMT_VIEW:
        return ViewMatrix;
    case EMT_MODEL:
        return ModelMatrix;
    case EMT_TEXTURE:
        return TextureMatrix;
    default:

        return core::matrixf();
    }
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::bindTexture(ITexture* Texture,u32 id)
{
    if(id < EMTN_TEXTURE_COUNT && ActiveTexture[id] != Texture)
    {
        if(ActiveTexture[id])
            ActiveTexture[id]->release();

        if(Texture)
            Texture->capture();

        ActiveTexture[id] = Texture;
    }
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::bindShaderProgram(IShaderProgram*)
{

}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::bindMaterial(IMaterial* Material)
{
    if(ActiveMaterial != Material)
    {
        if(Material)
            Material->capture();
        if(ActiveMaterial)
            ActiveMaterial->release();
    }


    switch(Material->getType())
    {


    case ERMT_DEFAULT:
    default:
    {
        bindTexture(Material->getTexture(),0);
    }
    break;

    }

    ActiveMaterial = Material;
}
void CBaseRenderer::setRenderTarget(ITexture * p_Target, u32 p_Attachment)
{
	if (QueryRendererFeature(ERF_RENDER_TO_TEXTURE))
	{

		//Init RenderTarget if need
		if (RenderTarget == nullptr)
			RenderTarget = createRenderTarget();

		RenderTarget->setTexture(p_Target, p_Attachment);

		setRenderTarget(RenderTarget);
	}
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setRenderTarget(IRenderTarget * p_RenderTarget)
{
	if (p_RenderTarget == nullptr)
	{
		if (ActiveRenderTarget)
		{
			ActiveRenderTarget->release();
			ActiveRenderTarget = nullptr;
		}
	}
	else
	{
		if (p_RenderTarget != ActiveRenderTarget)
		{
			if (ActiveRenderTarget)
				ActiveRenderTarget->release();
			if (p_RenderTarget)
				p_RenderTarget->capture();
			ActiveRenderTarget = p_RenderTarget;
		}
	}
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::begin_frame(bool clear_color_buffer,bool clear_zbuffer,core::color4f clear_color)
{
    u32 flags = 0;
    if(clear_color_buffer)
        flags |= ECF_COLOR_BUFFER;
    if(clear_zbuffer)
        flags |= ECF_DEPTH_BUFFER;

    clear(flags,clear_color);
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::end_frame()
{
    flush();
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::drawPrimitiveList(const SVertex* verticles,u32 VertexCount,E_PRIMITIVE_TYPE PrimitiveType,u32 VertexFormat)
{
    drawIndexedPrimitiveList(NULL,0,verticles,VertexCount,PrimitiveType,VertexFormat);
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::drawPoint(core::vector3f Position)
{
    drawArrays(0,1,NULL,&Position,NULL,NULL,NULL,renderer::EPT_POINT);
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::drawLine(core::vector3f Start,core::vector3f End)
{
    core::vector3f Line[2] = {Start,End};
    drawArrays(0,2,NULL,Line,NULL,NULL,NULL,renderer::EPT_LINE);
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::drawTriangle(core::vector3f Pos1,core::vector3f Pos2,core::vector3f Pos3)
{
    core::vector3f Triangle[3] = {Pos1,Pos2,Pos3};
    drawArrays(0,3,NULL,Triangle,NULL,NULL,NULL,renderer::EPT_TRIANGLE);
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::drawQuad(core::vector3f Pos1,core::vector3f Pos2,core::vector3f Pos3,core::vector3f Pos4)
{
    core::vector3f Quad[4] = {Pos1,Pos2,Pos3,Pos4};
    core::vector2f QuadTx[4] = {core::vector2f(0,1),core::vector2f(0,0),core::vector2f(1,0),core::vector2f(1,1)};
    drawArrays(0,4,NULL,Quad,QuadTx,NULL,NULL,renderer::EPT_QUAD);
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::drawScreenQuad(ITexture* Texture,core::color4f color)
{
    bindTexture(Texture,0);

    setTransform(core::matrixf(),EMT_PROJECTION);
    setTransform(core::matrixf(),EMT_VIEW);
    setTransform(core::matrixf(),EMT_MODEL);

    core::vector3f ScreenQuad_verticles[4] =
    {
        core::vector3f( 1,-1, 0),
        core::vector3f( 1, 1, 0),
        core::vector3f(-1,-1, 0),
        core::vector3f(-1, 1, 0)
    };

    core::vector2f ScreenQuad_texcoord[4] =
    {
        core::vector2f(1,0),
        core::vector2f(1,1),
        core::vector2f(0,0),
        core::vector2f(0,1)
    };

    core::color4f ScreenQuad_color[4] =
    {
        color,
        color,
        color,
        color
    };

    drawArrays(0,4,NULL,ScreenQuad_verticles,ScreenQuad_texcoord,NULL,ScreenQuad_color,EPT_TRIANGLE_STRIP);
}
//-----------------------------------------------------------------------------------------------
bool CBaseRenderer::isOk()
{
    return !(!noerror || exit);
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::__ClearCache()
{
	//Release active texture cache
	for (u32 i = 0; i < EMTN_TEXTURE_COUNT; i++)
		if (ActiveTexture[i])
			ActiveTexture[i]->release();
	//Release active material cache
	if (ActiveMaterial)
		ActiveMaterial->release();

	//Release RTT buffer
	if (RenderTarget)
		RenderTarget->release();
}
//-----------------------------------------------------------------------------------------------
}
}

