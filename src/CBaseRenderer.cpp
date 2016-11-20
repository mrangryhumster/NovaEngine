#include "CBaseRenderer.h"

namespace novaengine
{
namespace renderer
{

CBaseRenderer::CBaseRenderer(CPerformanceCounter* p_PerfCounter,window::IWindow* p_EngineWindow,SEngineConf p_EngineConf):
	m_RendererLastError(0),
	m_RendererReady(false),
    m_Renderer_Exit(false),
    m_EngineConf(p_EngineConf),
    m_EngineWindow(p_EngineWindow),
    m_RendererVersionName(nullptr),
    m_RendererVendorName(nullptr),
    m_RendererName(nullptr),
    m_PerformanceCounter(p_PerfCounter),
    m_ActiveProgram(nullptr),
	m_ActiveRenderTarget(nullptr),
	m_RenderTarget(nullptr)
{

    for(u32 i = 0; i < ERTU_TEXTURE_COUNT; i++)
        m_ActiveTexture[i] = nullptr;

}
//-----------------------------------------------------------------------------------------------
CBaseRenderer::~CBaseRenderer()
{

}
//-----------------------------------------------------------------------------------------------
u32 CBaseRenderer::getType()
{
    return ERT_NULL;
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setVSync(bool p_Flag)
{
    m_VSync = p_Flag;
}
//-----------------------------------------------------------------------------------------------
bool CBaseRenderer::getVSync()
{
    return m_VSync;
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setRenderState(u32 p_Flag,URenderStateValue p_StateValue)
{
    if(p_Flag < ERS_LAST_STATE)
        m_RenderStates[p_Flag] = p_StateValue;
}
//-----------------------------------------------------------------------------------------------
URenderStateValue CBaseRenderer::getRenderState(u32 p_Flag)
{
    if(p_Flag < ERS_LAST_STATE)
        return m_RenderStates[p_Flag];
    return 0;
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setViewport(core::rectu p_ViewPort)
{
    m_ViewportRect = p_ViewPort;
    m_ViewportSize = core::dim2u(p_ViewPort.X2 - p_ViewPort.X1, p_ViewPort.Y2 - p_ViewPort.Y1);
}
//-----------------------------------------------------------------------------------------------
core::rectu CBaseRenderer::getViewport()
{
    return m_ViewportRect;
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setTransform(const core::matrixf& mat,E_MATRIX_TYPE mtype)
{
    switch(mtype)
    {
    case EMT_PROJECTION:
        //--------------------------------------
        m_ProjectionMatrix = mat;
        m_MVPMatrix = m_ProjectionMatrix * m_ViewMatrix * m_ModelMatrix;
        //--------------------------------------
        return;

    case EMT_VIEW:
        //--------------------------------------
        m_ViewMatrix  = mat;
        m_MVPMatrix = m_ProjectionMatrix * m_ViewMatrix * m_ModelMatrix;
        //--------------------------------------
        return;

    case EMT_MODEL:
        //--------------------------------------
        m_ModelMatrix = mat;
        m_MVPMatrix = m_ProjectionMatrix * m_ViewMatrix * m_ModelMatrix;
        //--------------------------------------
        return;

    default:
        //--------------------------------------
        m_MVPMatrix = mat;
        //--------------------------------------
        return;
    }
}
//-----------------------------------------------------------------------------------------------
const core::matrixf& CBaseRenderer::getTransform(E_MATRIX_TYPE mtype)
{
    switch(mtype)
    {
    case EMT_PROJECTION:
        return m_ProjectionMatrix;
    case EMT_VIEW:
        return m_ViewMatrix;
    case EMT_MODEL:
        return m_ModelMatrix;
    default:
        return m_MVPMatrix;
    }
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::bindTexture(ITexture* p_Texture,u32 p_Unit)
{
    if(p_Unit < ERTU_TEXTURE_COUNT && m_ActiveTexture[p_Unit] != p_Texture)
    {
        if(m_ActiveTexture[p_Unit])
            m_ActiveTexture[p_Unit]->release();
        if(p_Texture)
			p_Texture->capture();

        m_ActiveTexture[p_Unit] = p_Texture;
    }
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::bindShaderProgram(IShaderProgram* p_ShaderProgram)
{
	if (p_ShaderProgram != m_ActiveProgram)
	{
		if (p_ShaderProgram)
			p_ShaderProgram->capture();
		if (m_ActiveProgram)
			m_ActiveProgram->release();

		m_ActiveProgram = p_ShaderProgram;
	}
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setRenderTarget(ITexture * p_Target, u32 p_Attachment)
{
	if (QueryRendererFeature(ERF_RENDER_TO_TEXTURE))
	{

		//Init RenderTarget if need
		if (m_RenderTarget == nullptr)
			m_RenderTarget = createRenderTarget();

		m_RenderTarget->setTexture(p_Target, p_Attachment);

		setRenderTarget(m_RenderTarget);
	}
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::setRenderTarget(IRenderTarget * p_RenderTarget)
{
	if (p_RenderTarget == nullptr)
	{
		if (m_ActiveRenderTarget)
		{
			m_ActiveRenderTarget->release();
			m_ActiveRenderTarget = nullptr;
		}
	}
	else
	{
		if (p_RenderTarget != m_ActiveRenderTarget)
		{
			if (m_ActiveRenderTarget)
				m_ActiveRenderTarget->release();
			if (p_RenderTarget)
				p_RenderTarget->capture();
			m_ActiveRenderTarget = p_RenderTarget;
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
bool CBaseRenderer::isReady()
{
	return m_RendererReady;
}
//-----------------------------------------------------------------------------------------------
bool CBaseRenderer::isOk()
{
    return !(m_RendererLastError || m_Renderer_Exit);
}
//-----------------------------------------------------------------------------------------------
void CBaseRenderer::ClearCache()
{
	//Release active texture cache
	for (u32 i = 0; i < ERTU_TEXTURE_COUNT; i++)
		if (m_ActiveTexture[i])
			m_ActiveTexture[i]->release();
	//Release RTT buffer
	if (m_RenderTarget)
		m_RenderTarget->release();
    //drop shader program
    if(m_ActiveProgram)
        m_ActiveProgram->release();
}
//-----------------------------------------------------------------------------------------------
}
}

