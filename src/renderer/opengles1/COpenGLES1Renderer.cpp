#ifdef NE_OPENGLES1_RENDERER

#include "COpenGLES1Renderer.h"

namespace novaengine
{
namespace renderer
{



COpenGLES1Renderer::COpenGLES1Renderer(window::IWindow* Window,SEngineConf EngineConfiguration):
    CBaseRenderer(Window,EngineConfiguration)
{
    setObjectName("renderer");
    LOG_ENGINE_DEBUG("COpenGLES1Renderer() begin\n");
    //---------------------------------------------------------------
    //preparing opengl context
#if defined(NE_WINDOW_ANDROID)
    LOG_ENGINE_DEBUG("Initializing opengl context for AndroidWindow\n");

    android_app* AndroidApp = (android_app*)Window->getWindowInternalVariable("android_app");
    if(!AndroidApp)
    {
        LOG_FATAL_ERROR("Cant request android_app\n");
        noerror = false;
    }

    const EGLint attribs[] =
    {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;

    if(noerror)
    {
    Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(Display, 0, 0);


    eglChooseConfig(Display, attribs, &config, 1, &numConfigs);


    eglGetConfigAttrib(Display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(AndroidApp->window, 0, 0, format);

    Surface = eglCreateWindowSurface(Display, config, AndroidApp->window, NULL);
    Context = eglCreateContext(Display, config, NULL, NULL);

    if (eglMakeCurrent(Display, Surface, Surface, Context) == EGL_FALSE)
     {
        LOG_FATAL_ERROR("Unable to eglMakeCurrent");

    }

    eglQuerySurface(Display, Surface, EGL_WIDTH, &w);
    eglQuerySurface(Display, Surface, EGL_HEIGHT, &h);
    LOG_ENGINE_DEBUG("ResSet(%d:%d)",w,h);
    setViewport(core::rectu(0,0,w,h));
    }

#endif
//---------------------------------------------------------------
//common openglgl initialization
    if(noerror)
    {
        LOG_ENGINE_DEBUG("Common opengl initialization\n");
        //------------------PrintBasicInformation
        versionname   =  reinterpret_cast<const char*>(glGetString(GL_VERSION));
        vendorname    =  reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        renderername  =  reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        //extensionlist =  reinterpret_cast<const char*>(glGetString(GL_FEATURE));
        //-------------------------------------------------------------------------
        LOG_INFO("Renderer type   : OpenGLES1Renderer (indev)\n");
        LOG_INFO("Renderer version: %s\n",versionname);
        LOG_INFO("Renderer name   : %s\n",renderername);
        LOG_INFO("Renderer vendor : %s\n",vendorname);
        //----------------------
        glClearColor(0,0,0,1);

        glFrontFace(GL_CCW);

        setRenderState(ERS_ENABLE_TEXTURES_2D,true);

        setRenderState(ERS_ENABLE_BLENDING,true);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        setRenderState(ERS_ENABLE_DEPTH_TEST,true);
        setRenderState(ERS_ENABLE_DEPTH_WRITE,true);
        setRenderState(ERS_DEPTH_TEST_MODE,EDTM_LEQUAL);

        setRenderState(ERS_ENABLE_CULL_FACE,true);
        setRenderState(ERS_CULL_FACE_MODE,ECFM_BACK);

        setRenderState(ERS_ENABLE_ALPHA_TEST,true);
        setRenderState(ERS_ALPHA_TEST_THRESHOLD,0.0f);

        GLfloat fogColor[4]= {0,0,0,1.0f};  // Fog Color

        setRenderState(ERS_LINE_WIDTH,1);
        setRenderState(ERS_POINT_SIZE,1);

        setTransform(core::matrixf(),EMT_PROJECTION);
        setTransform(core::matrixf(),EMT_VIEW);
        setTransform(core::matrixf(),EMT_MODEL);
        //----------------------
        FrameSize = Window->getResolution();
        //setViewport(core::rectu(0,0,FrameSize.width,FrameSize.height));

        memset(RendererClientStatesList,0,RCSL_STATES_COUNT);
//---------------------------------------------------------------
        ready = true;
    }
    LOG_ENGINE_DEBUG("COpenGLES1Renderer() end\n");
}
//--------------------------------------------------------------------------------------------------------
COpenGLES1Renderer::~COpenGLES1Renderer()
{

}
//--------------------------------------------------------------------------------------------------------
u32 COpenGLES1Renderer::getType()
{
    return ERT_OPENGL;
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::setVSync(bool flag)
{

}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::setRenderState(u32 flag,URenderStateValue value)
{
    CBaseRenderer::getRenderState(flag);
    switch(flag)
    {
    //!-------------------------------DEPTH_BUFFER
    case ERS_ENABLE_DEPTH_TEST:
        if(value.bool_value)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        break;
    case ERS_ENABLE_DEPTH_WRITE:
        if(value.bool_value)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
        break;
    case ERS_DEPTH_TEST_MODE:
        if(value.flags == EDTM_NEVER)     glDepthFunc(GL_NEVER);
        if(value.flags == EDTM_LESS)      glDepthFunc(GL_LESS);
        if(value.flags == EDTM_EQUAL)     glDepthFunc(GL_EQUAL);
        if(value.flags == EDTM_LEQUAL)    glDepthFunc(GL_LEQUAL);
        if(value.flags == EDTM_GREATER)   glDepthFunc(GL_GREATER);
        if(value.flags == EDTM_NOTEQUAL)  glDepthFunc(GL_NOTEQUAL);
        if(value.flags == EDTM_GEQUAL)    glDepthFunc(GL_GEQUAL);
        if(value.flags == EDTM_ALWAYS)    glDepthFunc(GL_ALWAYS);
        break;
    //!-------------------------------PIXEL_BLEND
    case ERS_ENABLE_BLENDING:
        if(value.bool_value)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
        break;

    case ERS_BLENDING_MODE:
        glBlendFunc(to_opengl_blendmodes((E_BLENDING_MODE)value.short_value[0]),to_opengl_blendmodes((E_BLENDING_MODE)value.short_value[1]));
        break;
    //!-------------------------------TEXTURE_MAPPING
    case ERS_ENABLE_TEXTURES_2D:
        if(value.bool_value)
            glEnable(GL_TEXTURE_2D);
        else
            glDisable(GL_TEXTURE_2D);
        break;

    //!-------------------------------WIREFRAME_RENDER
    case ERS_ENABLE_WIREFRAME:
        //unsupported
        break;

    //!-------------------------------CULL_FACE
    case ERS_ENABLE_CULL_FACE:
        if(value.bool_value)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
        break;

    case ERS_CULL_FACE_MODE:
        if(value.flags == ECFM_BACK)      glCullFace(GL_BACK);
        if(value.flags == ECFM_FRONT)     glCullFace(GL_FRONT);
        if(value.flags == ECFM_ALL)       glCullFace(GL_FRONT_AND_BACK);
        break;
    //!-------------------------------ALPHA_TEST
    case ERS_ENABLE_ALPHA_TEST:
        if(value.bool_value)
            glEnable(GL_ALPHA_TEST);
        else
            glDisable(GL_ALPHA_TEST);
        break;
    case ERS_ALPHA_TEST_THRESHOLD:
        glAlphaFunc(GL_GREATER,value.float_value);
        break;
    //!-------------------------------FOG
    case ERS_ENABLE_FOG:
        if(value.bool_value)
            glEnable(GL_FOG);
        else
            glDisable(GL_FOG);
        break;
    case ERS_FOG_MODE:
        break;
    case ERS_FOG_START_DISTANCE:
        glFogf(GL_FOG_START, value.float_value);
        break;
    case ERS_FOG_END_DISTANCE:
        glFogf(GL_FOG_END, value.float_value);
        break;
    case ERS_FOG_DENSITY:
        glFogf(GL_FOG_DENSITY, value.float_value);
        break;
    //!-------------------------------LINE_WIDTH
    case ERS_LINE_WIDTH:
        glLineWidth(value.int_value);
        break;
    //!-------------------------------POINT_SIZE
    case ERS_POINT_SIZE:
        glPointSize(value.int_value);
        break;
    default:
        break;
    }

}
//--------------------------------------------------------------------------------------------------------
URenderStateValue COpenGLES1Renderer::getRenderState(u32 flag)
{
    return CBaseRenderer::getRenderState(flag);
}
//--------------------------------------------------------------------------------------------------------
bool COpenGLES1Renderer::isSupported(E_RENDERER_FEATURE ext)
{
    return false;
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::setViewport(core::rectu nvp)
{
    glViewport(nvp.X1,nvp.Y1,nvp.X2,nvp.Y2);
    ViewportRect = nvp;
    ViewportSize = core::dim2u(nvp.X2 - nvp.X1,nvp.Y2 - nvp.Y1);
}
//--------------------------------------------------------------------------------------------------------
core::rectu COpenGLES1Renderer::getViewport()
{
    return ViewportRect;
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::setTransform(const core::matrixf& mat,E_MATRIX_TYPE mtype)
{

    switch(mtype)
    {
    case EMT_PROJECTION:
        //--------------------------------------
        if(ProjectionMatrix == mat)
            return;
        //--------------------------------------
        ProjectionMatrix = mat;
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(ProjectionMatrix.getPointer());
        //--------------------------------------
        break;
    case EMT_VIEW:
    case EMT_MODEL:
        //--------------------------------------
        if(mtype == EMT_VIEW)
        {
            if(ViewMatrix == mat)
                return;
            ViewMatrix  = mat;
        }
        else

        {
            if(ModelMatrix == mat)
                return;
            ModelMatrix = mat;
        }
        //--------------------------------------
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf((ViewMatrix * ModelMatrix).getPointer());
        //--------------------------------------
        break;
    case EMT_TEXTURE:
        //--------------------------------------
        if(TextureMatrix == mat)
            return;
        //--------------------------------------
        TextureMatrix = mat;
        glMatrixMode(GL_TEXTURE);
        glLoadMatrixf(TextureMatrix.getPointer());
        //--------------------------------------
        break;
    }
}
//--------------------------------------------------------------------------------------------------------
const core::matrixf COpenGLES1Renderer::getTransform(E_MATRIX_TYPE mtype)
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
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::resetTransform(E_MATRIX_TYPE mtype)
{
    switch(mtype)
    {
    case EMT_PROJECTION:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //--------------------------------------
        break;
    case EMT_VIEW:
    case EMT_MODEL:
        //--------------------------------------
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //--------------------------------------
        break;
    case EMT_TEXTURE:
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        //--------------------------------------
        break;
    }
}
//--------------------------------------------------------------------------------------------------------
IShaderProgram* COpenGLES1Renderer::GenShaderProgram()
{
    return NULL;
}
//--------------------------------------------------------------------------------------------------------
IVertexBuffer* COpenGLES1Renderer::GenVertexBuffer()
{
    return new COpenGLES1VertexBuffer();
}
//--------------------------------------------------------------------------------------------------------
ITexture* COpenGLES1Renderer::GenTexture(IImage* img,STextureParameters params)
{
    return NULL;
}
//--------------------------------------------------------------------------------------------------------
IImage*   COpenGLES1Renderer::GenImage(ITexture* tx)
{
    return NULL;
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::bindTexture(ITexture* Texture,u32 id)
{
    CBaseRenderer::bindTexture(Texture,id);
    switch(id)
    {
    case 0:
        glActiveTexture(GL_TEXTURE0);
        break;
    case 1:
        glActiveTexture(GL_TEXTURE1);
        break;
    case 2:
        glActiveTexture(GL_TEXTURE2);
        break;
    case 3:
        glActiveTexture(GL_TEXTURE3);
        break;
    case 4:
        glActiveTexture(GL_TEXTURE4);
        break;
    case 5:
        glActiveTexture(GL_TEXTURE5);
        break;
    case 6:
        glActiveTexture(GL_TEXTURE6);
        break;
    case 7:
        glActiveTexture(GL_TEXTURE7);
        break;
    default:
        glActiveTexture(GL_TEXTURE0);
    }
/*
    if(Texture)
        glBindTexture(GL_TEXTURE_2D,((COpenGLTexture*)Texture)->getTexture());
    else
        glBindTexture(GL_TEXTURE_2D,0);
*/
}
//-------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::bindShaderProgram(IShaderProgram* ShaderProgram)
{
    return;
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::bindMaterial(IMaterial* Material)
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
        bindTexture(Material->getTexture(0),0);
    }
    }

    //glColor4f(Material->getColor().Red,Material->getColor().Green,Material->getColor().Blue,Material->getColor().Alpha);
    core::color4f DiffuseColor = Material->getDiffuseColor();
    //glColor4fv((float*)&DiffuseColor);
    ActiveMaterial = Material;
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::setRenderTarget(ITexture* target,E_RENDER_TARGET_TYPE target_type)
{
    return;
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::clear(u32 flags,core::color4f clear_color)
{
    glClearColor(clear_color.r,clear_color.g,clear_color.b,clear_color.a);
    GLbitfield GL_FLAGS = 0;

    if(flags & ECF_COLOR_BUFFER)
        GL_FLAGS |= GL_COLOR_BUFFER_BIT;
    if(flags & ECF_DEPTH_BUFFER)
        GL_FLAGS |= GL_DEPTH_BUFFER_BIT;

    glClear(GL_FLAGS);
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::flush()
{
    eglSwapBuffers(Display, Surface);
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::drawVertexBuffer(IVertexBuffer* VertexBuffer)
{
          drawArrays(
            VertexBuffer->getIndicesCount(),
            VertexBuffer->getPositionsCount(),
            VertexBuffer->getIndices(),
            VertexBuffer->getPositions(),
            VertexBuffer->getUVs(),
            VertexBuffer->getNormals(),
            VertexBuffer->getColors(),
            (E_PRIMITIVE_TYPE)VertexBuffer->getPrimitiveType()
        );
}
//-------------------------------a-------------------------------------------------------------------------
void COpenGLES1Renderer::drawPrimitiveList(const SVertex* verticles,u32 VertexCount,E_PRIMITIVE_TYPE PrimitiveType,u32 VertexFormat)
{
    drawIndexedPrimitiveList(NULL,0,verticles,VertexCount,PrimitiveType,VertexFormat);
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::drawIndexedPrimitiveList(const u16* Index,u16 IndexCount,const SVertex* verticles,u32 VertexCount,E_PRIMITIVE_TYPE PrimitiveType,u32 VertexFormat)
{

    if(verticles == NULL)
        return;
    if(VertexCount == 0)
        return;

    //------------------------------------------------------------
    bool have_verticles = (VertexFormat & EVF_VERTEX);
    bool have_texcoords = (VertexFormat & EVF_TEXCOORD);
    bool have_normals   = (VertexFormat & EVF_NORMAL);
    bool have_colors    = (VertexFormat & EVF_VERTEX_COLOR);
    //!Enable/disable client states for drawing
    enable_client_states(have_verticles,have_texcoords,have_normals,have_colors);

    //!Send verticles to video adapter
    if(have_verticles)
        glVertexPointer(    3,  GL_FLOAT,           sizeof(SVertex),    &verticles[0].Position  );
    if(have_texcoords)
        glTexCoordPointer(  2,  GL_FLOAT,           sizeof(SVertex),    &verticles[0].TexCoord  );
    if(have_normals)
        glNormalPointer(        GL_FLOAT,           sizeof(SVertex),    &verticles[0].Normal    );
    if(have_colors)
        glColorPointer(     4,  GL_UNSIGNED_BYTE,   sizeof(SVertex),    &verticles[0].Color);
    //------------------------------------------------------------
    GLenum GLPrimitiveType   = 0;
    u32    VertexInPrimitive = 0;
    //!Convert E_PRIMITIVE_TYPE to GLenum
    to_opengl_primitive((E_PRIMITIVE_TYPE)PrimitiveType,GLPrimitiveType,VertexInPrimitive);
    //----------------------------------------------
    if(have_verticles && (Index != NULL || IndexCount != 0))
        glDrawElements(GLPrimitiveType,IndexCount,GL_UNSIGNED_SHORT,Index);
    else if(have_verticles)
        glDrawArrays(GLPrimitiveType,0,VertexCount);
    //----------------------------------------------
    RendererPerformanceCounter->register_draw(VertexCount,VertexInPrimitive);
    //----------------------------------------------

}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::drawArrays(u16 indices_count,u32 vertex_count,const u16* indices,const core::vector3f* verticles,const core::vector2f* texverts,const core::vector3f* normals,const core::color4u* colors,E_PRIMITIVE_TYPE PrimitiveType)
{
    //------------------------------------------------------------
    bool have_verticles = (verticles!=NULL);
    bool have_texcoords = (texverts !=NULL);
    bool have_normals   = (normals  !=NULL);
    bool have_colors    = (colors   !=NULL);

    //!Enable/disable client states for drawing
    enable_client_states(have_verticles,have_texcoords,have_normals,have_colors);
    //!Send verticles to vram
    if(have_verticles)
        glVertexPointer(    3,  GL_FLOAT,           0,  verticles);
    if(have_texcoords)
        glTexCoordPointer(  2,  GL_FLOAT,           0,  texverts);
    if(have_normals)
        glNormalPointer(        GL_FLOAT,           0,  normals);
    if(have_colors)
        glColorPointer(     4,  GL_UNSIGNED_BYTE,   0,  colors);
    //------------------------------------------------------------
    GLenum GLPrimitiveType   = 0;
    u32    VertexInPrimitive = 0;
    //!Convert E_PRIMITIVE_TYPE to GLenum
    to_opengl_primitive((E_PRIMITIVE_TYPE)PrimitiveType,GLPrimitiveType,VertexInPrimitive);
    //----------------------------------------------
    if(indices != NULL || indices_count != 0)
        glDrawElements(GLPrimitiveType,indices_count,GL_UNSIGNED_SHORT,indices);
    else
        glDrawArrays(GLPrimitiveType,0,vertex_count);
    //----------------------------------------------
    RendererPerformanceCounter->register_draw(vertex_count,VertexInPrimitive);
    //----------------------------------------------
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::drawArrays(u16 indices_count,u32 vertex_count,const u16* indices,const core::vector2f* verticles,const core::vector2f* texverts,const core::color4u* colors,E_PRIMITIVE_TYPE PrimitiveType)
{
    //------------------------------------------------------------
    bool have_verticles = (verticles!=NULL);
    bool have_texcoords = (texverts !=NULL);
    bool have_colors    = (colors   !=NULL);

    //!Enable/disable client states for drawing
    enable_client_states(have_verticles,have_texcoords,false,have_colors);

    //!Send verticles to video adapter
    if(have_verticles)
        glVertexPointer(    2,  GL_FLOAT,           0,  verticles);
    if(have_texcoords)
        glTexCoordPointer(  2,  GL_FLOAT,           0,  texverts);
    if(have_colors)
        glColorPointer(     4,  GL_UNSIGNED_BYTE,   0,  colors);
    //------------------------------------------------------------
    GLenum GLPrimitiveType   = 0;
    u32    VertexInPrimitive = 0;
    //!Convert E_PRIMITIVE_TYPE to GLenum
    to_opengl_primitive((E_PRIMITIVE_TYPE)PrimitiveType,GLPrimitiveType,VertexInPrimitive);
    //----------------------------------------------
    if(indices != NULL || indices_count != 0)
        glDrawElements(GLPrimitiveType,indices_count,GL_UNSIGNED_SHORT,indices);
    else
        glDrawArrays(GLPrimitiveType,0,vertex_count);
    //----------------------------------------------
    RendererPerformanceCounter->register_draw(vertex_count,VertexInPrimitive);
    //----------------------------------------------
}
//--------------------------------------------------------------------------------------------------------
bool COpenGLES1Renderer::isOk()
{
    return !(!noerror || exit);
}
//--------------------------------------------------------------------------------------------------------
bool COpenGLES1Renderer::update()
{
    //return isOk();
    GLenum last_error = glGetError();

    while(last_error != GL_NO_ERROR)
    {
        switch(last_error)
        {
        case GL_INVALID_ENUM:
            LOG_ENGINE_DEBUG("OpenGL error - GL_INVALID_ENUM\n");
            break;
        case GL_INVALID_VALUE:
            LOG_ENGINE_DEBUG("OpenGL error - GL_INVALID_VALUE\n");
            break;
        case GL_INVALID_OPERATION:
            LOG_ENGINE_DEBUG("OpenGL error - GL_INVALID_OPERATION\n");
            break;
        case GL_STACK_OVERFLOW:
            LOG_ENGINE_DEBUG("OpenGL error - GL_STACK_OVERFLOW\n");
            break;
        case GL_STACK_UNDERFLOW:
            LOG_ENGINE_DEBUG("OpenGL error - GL_STACK_UNDERFLOW\n");
            break;
        case GL_OUT_OF_MEMORY:
            LOG_ENGINE_DEBUG("OpenGL error - GL_OUT_OF_MEMORY\n");
            break;
        }
    }


    return isOk();
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::to_opengl_primitive(E_PRIMITIVE_TYPE engine_primitive,u32& gl_primitive,u32& vertexperprimitive)
{
    switch(engine_primitive)
    {
    case EPT_POINT:
        gl_primitive = GL_POINTS;
        vertexperprimitive = 1;
        break;
    case EPT_LINE:
        gl_primitive = GL_LINES;
        vertexperprimitive = 2;
        break;
    case EPT_LINE_STRIP:
        gl_primitive = GL_LINE_STRIP;
        vertexperprimitive = 2;
        break;
    case EPT_TRIANGLE:
        gl_primitive = GL_TRIANGLES;
        vertexperprimitive = 3;
        break;
    case EPT_TRIANGLE_STRIP:
        gl_primitive = GL_TRIANGLE_STRIP;
        vertexperprimitive = 2;
        break;
    default:
        gl_primitive = GL_TRIANGLES;
        vertexperprimitive = 3;
        break;
    }
}
//--------------------------------------------------------------------------------------------------------
u32 COpenGLES1Renderer::to_opengl_blendmodes(E_BLENDING_MODE engine_mode)
{
    switch(engine_mode)
    {
    case EBM_ZERO:
        return GL_ZERO;
        break;
    case EBM_ONE:
        return GL_ONE;
        break;
    case EBM_SRC_COLOR:
        return GL_SRC_COLOR;
        break;
    case EBM_ONE_MINUS_SRC_COLOR:
        return GL_ONE_MINUS_SRC_COLOR;
        break;
    case EBM_DST_COLOR:
        return GL_DST_COLOR;
        break;
    case EBM_ONE_MINUS_DST_COLOR:
        return GL_ONE_MINUS_DST_COLOR;
        break;
    case EBM_SRC_ALPHA:
        return GL_SRC_ALPHA;
        break;
    case EBM_ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
        break;
    case EBM_DST_ALPHA:
        return GL_DST_ALPHA;
        break;
    case EBM_ONE_MINUS_DST_ALPHA:
        return GL_ONE_MINUS_DST_ALPHA;
        break;
    }
    return GL_ONE;
}
//--------------------------------------------------------------------------------------------------------
void COpenGLES1Renderer::enable_client_states(bool vert,bool tex,bool norm,bool color)
{
    //---------------------------------------------------------VertexBuffer
    if(vert)
    {
        if(RendererClientStatesList[RCSL_VERTEX_ARRAY] == false)
        {
            RendererClientStatesList[RCSL_VERTEX_ARRAY] = true;
            glEnableClientState(GL_VERTEX_ARRAY);
        }
    }
    else
    {
        if(RendererClientStatesList[RCSL_VERTEX_ARRAY] == true)
        {
            RendererClientStatesList[RCSL_VERTEX_ARRAY] = false;
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }
    //---------------------------------------------------------TextureCoordArray
    if(tex)
    {
        if(RendererClientStatesList[RCSL_TEXCORD_ARRAY] == false)
        {
            RendererClientStatesList[RCSL_TEXCORD_ARRAY] = true;
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    else
    {
        if(RendererClientStatesList[RCSL_TEXCORD_ARRAY] == true)
        {
            RendererClientStatesList[RCSL_TEXCORD_ARRAY] = false;
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    //---------------------------------------------------------NormalArray
    if(norm)
    {
        if(RendererClientStatesList[RCSL_NORMAL_ARRAY] == false)
        {
            RendererClientStatesList[RCSL_NORMAL_ARRAY] = true;
            glEnableClientState(GL_NORMAL_ARRAY);
        }
    }
    else
    {
        if(RendererClientStatesList[RCSL_NORMAL_ARRAY] == true)
        {
            RendererClientStatesList[RCSL_NORMAL_ARRAY] = false;
            glDisableClientState(GL_NORMAL_ARRAY);
        }
    }
    //---------------------------------------------------------ColorArray
    if(color)
    {
        if(RendererClientStatesList[RCSL_COLOR_ARRAY] == false)
        {
            RendererClientStatesList[RCSL_COLOR_ARRAY] = true;
            glEnableClientState(GL_COLOR_ARRAY);
        }
    }
    else
    {
        if(RendererClientStatesList[RCSL_COLOR_ARRAY] == true)
        {
            RendererClientStatesList[RCSL_COLOR_ARRAY] = false;
            glDisableClientState(GL_COLOR_ARRAY);
        }
    }
}
//--------------------------------------------------------------------------------------------------------
}
}

#endif // NE_OPENGLES1_RENDERER

