#include "COpenGLRenderer.h"

namespace novaengine
{
namespace renderer
{



COpenGLRenderer::COpenGLRenderer(CPerformanceCounter* PerformanceCounter,window::IWindow* Window,SEngineConf EngineConfiguration):
    CBaseRenderer(PerformanceCounter,Window,EngineConfiguration)
{
    setObjectName("COpenGLRenderer");
    LOG_ENGINE_DEBUG("COpenGLRenderer() begin\n");
    //---------------------------------------------------------------
    //preparing opengl context
#ifdef NE_WINDOW_WIN32
    LOG_ENGINE_DEBUG("Initializing opengl context for Win32Window\n");
    if(EngineConfiguration.ExternalWindowPointer == nullptr)
    {
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
        PIXELFORMATDESCRIPTOR pfd;
        int pFormat;
        DWORD dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;

        hWnd = *(HWND*)Window->getWindowInternalVariable("HWND");
        hDC  = GetDC(hWnd);

        if(!hDC)
        {
            LOG_FATAL_ERROR("Cant request DC.\n");
            noerror = false;
        }

        if(EngineConfiguration.DoubleBuf)
            dwFlags |= PFD_DOUBLEBUFFER; //!< DoubleBuf

        ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = dwFlags;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = EngineConfiguration.Bits;
        pfd.cDepthBits = EngineConfiguration.ZBits;
        pfd.iLayerType = PFD_MAIN_PLANE;

        //-------------------------------------------------------------------------------------------
        HWND temp_wnd = NULL;
        HDC temp_dc   = NULL;
        HGLRC temp_rc = NULL;
        //! Create a temp window with gl context(temp)
        temp_wnd = CreateWindowEx(0,"novaengine","window",0,0,0,0,0,0,0,0,0);
        if(!temp_wnd)
        {
            LOG_ERROR("Cant create temp window.\n");
            pFormat = ChoosePixelFormat(hDC,&pfd);
        }
        else
        {
            temp_dc = GetDC(temp_wnd);
            pFormat = ChoosePixelFormat(temp_dc,&pfd);
            if(!SetPixelFormat(temp_dc,pFormat,&pfd))
                LOG_ERROR("Unable to set PixelFormat.\n");
            //! create and enable opengl context
            temp_rc = wglCreateContext(temp_dc);
        }

        if(!temp_rc)
        {
            LOG_ERROR("Cannot create temp render context.\n");
            pFormat = ChoosePixelFormat(hDC,&pfd);
        }
        else
        {

            wglMakeCurrent(temp_dc,temp_rc);
            wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
        }
        if (!wglChoosePixelFormatARB)
        {
            LOG_ERROR("Can't get wglChoosePixelFormatARB...\n");
            pFormat = ChoosePixelFormat(hDC,&pfd);
        }
        else
        {
            bool valid;
            UINT numFormats;
            float fAttributes[] = {0,0};

            int iAttributes[] =
            {
                WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
                WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
                WGL_COLOR_BITS_ARB,(EngineConfiguration.Bits==32) ? 24 : 16,
                WGL_ALPHA_BITS_ARB,(EngineConfiguration.Bits==32) ? 8  : 0,
                WGL_DEPTH_BITS_ARB,(int)EngineConfiguration.ZBits,
                WGL_STENCIL_BITS_ARB,0,
                WGL_DOUBLE_BUFFER_ARB, (EngineConfiguration.DoubleBuf) ? GL_TRUE : GL_FALSE,
                WGL_SAMPLE_BUFFERS_ARB,(EngineConfiguration.AntiAlias) ? GL_TRUE : GL_FALSE,
                WGL_SAMPLES_ARB,(int)EngineConfiguration.AntiAlias,
                0,0
            };
            valid = wglChoosePixelFormatARB(temp_dc,iAttributes,fAttributes,1,&pFormat,&numFormats) != 0;
            if ( !(valid && numFormats >= 1) )
            {
                pFormat = ChoosePixelFormat(hDC,&pfd);
                LOG_ERROR("Can't set pixel format with wglChoosePixelFormatARB...\n");
            }
        }

        wglMakeCurrent(NULL, NULL);
        if(temp_rc)
            wglDeleteContext(temp_rc);
        if(temp_dc)
            ReleaseDC(temp_wnd, temp_dc);
        if(temp_wnd)
            DestroyWindow(temp_wnd);
        //-------------------------------------------------------------------------------------------

        pFormat = ChoosePixelFormat(hDC,&pfd);
        if(!SetPixelFormat(hDC,pFormat,&pfd))
        {
            LOG_FATAL_ERROR("Unable to set PixelFormat.\n");
            noerror = false;
            return;
        }

        //! create and enable opengl context
        hRC = wglCreateContext(hDC);
        if(!hRC)
        {
            LOG_FATAL_ERROR("Cannot create render context [err:%d]\n",GetLastError());
            noerror = false;
            return;
        }

        wglMakeCurrent(hDC,hRC);
        setVSync(EngineConfiguration.VSync);
    }
    else
    {
        noerror = false;
        return;
    }
#endif // NE_WINDOW_WIN32
//---------------------------------------------------------------
//common openglgl initialization
    if(noerror)
    {
        //------------------GLEW
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            LOG_FATAL_ERROR("Cannot init GLEW %s\n",glewGetErrorString(err));
            noerror = false;
            return;
        }
        else
        {
            LOG_INFO("Using GLEW v.%s\n",glewGetString(GLEW_VERSION));
        }

        LOG_ENGINE_DEBUG("Common opengl initialization\n");
        //------------------PrintBasicInformation
        versionname   =  reinterpret_cast<const char*>(glGetString(GL_VERSION));
        vendorname    =  reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        renderername  =  reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        extensionlist =  reinterpret_cast<const char*>(glGetString(GL_FEATURE));
        //-------------------------------------------------------------------------
        LOG_INFO("Renderer type   : OpenGLRenderer\n");
        LOG_INFO("Renderer version: %s\n",versionname);
        LOG_INFO("Renderer name   : %s\n",renderername);
        LOG_INFO("Renderer vendor : %s\n",vendorname);
        //----------------------
        glClearColor(0,0,0,1);
        glClearDepth(1.0f);

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

        glFogi(GL_FOG_MODE, GL_LINEAR);     // Fog Mode
        glFogfv(GL_FOG_COLOR, fogColor);    // Set Fog Color
        glFogf(GL_FOG_DENSITY, 0.35f);      // How Dense Will The Fog Be
        glHint(GL_FOG_HINT, GL_DONT_CARE);  // Fog Hint Value
        glFogf(GL_FOG_START, 50.0f);         // Fog Start Depth
        glFogf(GL_FOG_END, 200.0f);         // Fog End Depth
        glEnable(GL_FOG);                   // Enables GL_FOG

        setRenderState(ERS_LINE_WIDTH,1);
        setRenderState(ERS_POINT_SIZE,1);

        setTransform(core::matrixf(),EMT_PROJECTION);
        setTransform(core::matrixf(),EMT_VIEW);
        setTransform(core::matrixf(),EMT_MODEL);
        //----------------------
        FrameSize = Window->getResolution();
        setViewport(core::rectu(0,0,FrameSize.width,FrameSize.height));

        memset(RendererClientStatesList,0,RCSL_STATES_COUNT);
        //---------------------------------------------------------------
        active_framebuffer_object = 0;
        //---------------------------------------------------------------
        ready = true;
    }
    LOG_ENGINE_DEBUG("COpenGLRenderer() end\n");
}
//-----------------------------------------------------------------------------------------------
COpenGLRenderer::~COpenGLRenderer()
{
#ifdef NE_WINDOW_WIN32
    wglMakeCurrent(NULL, NULL);
    if(hRC && !wglDeleteContext(hRC))
        LOG_FATAL_ERROR("Cant delete GL context [err:%d]\n",GetLastError());
    if(hDC && !ReleaseDC(hWnd, hDC))
        LOG_FATAL_ERROR("Cant Release DC[err:unknown]\n");
#endif // NE_WINDOW_WIN32
}
//-----------------------------------------------------------------------------------------------
u32 COpenGLRenderer::getType()
{
    return ERT_OPENGL;
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::setVSync(bool flag)
{
#ifdef NE_WINDOW_WIN32
    typedef BOOL (APIENTRY * WGLSWAPINTERVALEXTFUNC)(int);
    WGLSWAPINTERVALEXTFUNC wglSwapIntervalEXT = WGLSWAPINTERVALEXTFUNC(wglGetProcAddress("wglSwapIntervalEXT"));

    if(wglSwapIntervalEXT)
    {
        wglSwapIntervalEXT(flag);
        VSync = flag;
    }
    else
    {
        LOG_ERROR("VSync not supported? it's something new....\n");
    }
#endif // NE_WINDOW_WIN32
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::setRenderState(u32 flag,URenderStateValue value)
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
        if(value.bool_value)
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
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
        if(value.int_value == 0) glFogi(GL_FOG_MODE,GL_LINEAR);
        if(value.int_value == 1) glFogi(GL_FOG_MODE,GL_EXP);
        if(value.int_value == 2) glFogi(GL_FOG_MODE,GL_EXP2);
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
    case ERS_FOG_COLOR:
        glFogfv(GL_FOG_COLOR, value.color4f);
        break;
    //!-------------------------------LINE_WIDTH
    case ERS_LINE_WIDTH:
        glLineWidth(value.float_value);
        break;
    //!-------------------------------POINT_SIZE
    case ERS_POINT_SIZE:
        glPointSize(value.float_value);
        break;
    default:
        break;
    }

}
//-----------------------------------------------------------------------------------------------
URenderStateValue COpenGLRenderer::getRenderState(u32 flag)
{
    return CBaseRenderer::getRenderState(flag);
}
//-----------------------------------------------------------------------------------------------
s32  COpenGLRenderer::QueryRendererFeature(E_RENDERER_FEATURE feature)
{
    switch(feature)
    {
    case ERF_RENDER_GRAPHICS:
        return true;
    case ERF_VBO:
        return (GLEW_ARB_vertex_buffer_object);
    case ERF_RENDER_TO_TEXTURE:
        return (GLEW_ARB_framebuffer_object);
    case ERF_SHADERS_1_0:
        return (GL_VERSION_2_0);
    case ERF_SHADERS_2_0:
        return (GL_VERSION_3_0);
    case ERF_SHADERS_3_0:
        return (GL_VERSION_4_0);
    }
    return false;
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::setViewport(core::rectu nvp)
{
    glViewport(nvp.X1,nvp.Y1,nvp.X2,nvp.Y2);
    ViewportRect = nvp;
    ViewportSize = core::dim2u(nvp.X2 - nvp.X1,nvp.Y2 - nvp.Y1);
}
//-----------------------------------------------------------------------------------------------
core::rectu COpenGLRenderer::getViewport()
{
    return ViewportRect;
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::setTransform(const core::matrixf& mat,E_MATRIX_TYPE mtype)
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
//-----------------------------------------------------------------------------------------------
const core::matrixf COpenGLRenderer::getTransform(E_MATRIX_TYPE mtype)
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
void COpenGLRenderer::resetTransform(E_MATRIX_TYPE mtype)
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
//-----------------------------------------------------------------------------------------------
IShaderProgram* COpenGLRenderer::GenShaderProgram()
{
    return new COpenGLShaderProgram();
}
//-----------------------------------------------------------------------------------------------
IMeshBuffer* COpenGLRenderer::GenMeshBuffer()
{
    return new COpenGLMeshBuffer();
}
//-----------------------------------------------------------------------------------------------
ITexture* COpenGLRenderer::GenTexture(IImage* img,STextureParameters params)
{
    if(!img)
        return NULL;

    u32 time = time::getRealTime();
    COpenGLTexture* OpenGLTexture = new COpenGLTexture(this,img,params);
    LOG_INFO("Texture generated : tx_id:%d [%d ms]\n",OpenGLTexture->getTexture(),time::getRealTime() - time);
    return OpenGLTexture;
}
//-----------------------------------------------------------------------------------------------
IImage*   COpenGLRenderer::GenImage(ITexture* tx)
{
    tx->lock();
    tx->unlock();

    return NULL;
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::bindTexture(ITexture* Texture,u32 id)
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

    if(Texture)
        glBindTexture(GL_TEXTURE_2D,((COpenGLTexture*)Texture)->getTexture());
    else
        glBindTexture(GL_TEXTURE_2D,0);

}
//-------------------------------------------------------------------------------------------------------
void COpenGLRenderer::bindShaderProgram(IShaderProgram* ShaderProgram)
{
    if(ShaderProgram != NULL)
    {
        COpenGLShaderProgram* Program = reinterpret_cast<COpenGLShaderProgram*>(ShaderProgram);
        if(Program->getLastError() == 0)
            glUseProgram(Program->getProgramID());
    }
    else
    {
        glUseProgram(0);
    }
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::bindMaterial(IMaterial* Material)
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
    core::color4f color = Material->getDiffuseColor();
    glColor4fv((float*)&color);
    ActiveMaterial = Material;
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::setRenderTarget(ITexture* target,u32 target_type)
{
    if(GLEW_ARB_framebuffer_object)
    {
        if(active_framebuffer_object == 0)
            glGenFramebuffers(1,&active_framebuffer_object);


        if(target_type == 0)
        {
            glBindFramebuffer(GL_FRAMEBUFFER,0);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER,active_framebuffer_object);


            GLuint gl_texture = 0;
            if(target != NULL)
            {
                gl_texture = reinterpret_cast<COpenGLTexture*>(target)->getTexture();
            }

            if (target_type >= ERTT_COLOR_BUFFER_0 && target_type <= ERTT_COLOR_BUFFER_15)
            {
				if (RTT_color_buffers[target_type - 1])
					RTT_color_buffers[target_type - 1]->release();
				if (target)
					target->capture();
				RTT_color_buffers[target_type - 1] = target;

				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (target_type - 1), gl_texture, 0);

				std::vector<GLenum> RenderBuffers;
				for (u32 i = 0; i < ERTT_COLOR_BUFFERS_COUNT; i++)
					if (RTT_color_buffers[i])
						RenderBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);

				glDrawBuffers(RenderBuffers.size(), RenderBuffers.data());
            }
            else if (target_type == ERTT_DEPTH_BUFFER)
            {
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, gl_texture, 0);
            }
            else if (target_type == ERTT_STENCIL_BUFFER)
            {
				//not implemented yet
            }


            //----------------------------------------------------------------------
            //FBO error check block
            GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if(err != GL_FRAMEBUFFER_COMPLETE)
            {
                LOG_ERROR("Something wrong with framebuffer...\n");
                switch(err)
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
                return;
            }
            //----------------------------------------------------------------------
        }
    }
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::clear(u32 flags,core::color4f clear_color)
{
    glClearColor(clear_color.r,clear_color.g,clear_color.b,clear_color.a);
    GLbitfield GL_FLAGS = 0;

    if(flags & ECF_COLOR_BUFFER)
        GL_FLAGS |= GL_COLOR_BUFFER_BIT;
    if(flags & ECF_DEPTH_BUFFER)
        GL_FLAGS |= GL_DEPTH_BUFFER_BIT;

    glClear(GL_FLAGS);
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::flush()
{
#ifdef NE_WINDOW_WIN32
    SwapBuffers(hDC);
#endif // NE_WINDOW_WIN32
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::drawMeshBuffer(IMeshBuffer* Buffer)
{
    COpenGLMeshBuffer* MeshBuffer = reinterpret_cast<COpenGLMeshBuffer*>(Buffer);

    if(MeshBuffer->getUpdateRequest())
        MeshBuffer->update();

    if(MeshBuffer->getMaterial())
        bindMaterial(MeshBuffer->getMaterial());

    if(MeshBuffer->getMappingHint() == EMBMH_DEFAULT)
    {
        const SVertexFormat& Format = MeshBuffer->getVertexFormat();
        //------------------------------------------------------------
        bool have_verticles = (Format.getFlags() & EVA_POSITION) != 0;
        bool have_texcoords = (Format.getFlags() & EVA_TEXCOORD) != 0;
        bool have_normals   = (Format.getFlags() & EVA_NORMAL)   != 0;
        bool have_colors    = (Format.getFlags() & EVA_COLOR)    != 0;

        //!If no positions in MeshBuffer then nothing to render
        if(have_verticles == false)
            return;

        //!Enable/disable client states for drawing
        enable_client_states(have_verticles,have_texcoords,have_normals,have_colors);
        //!Send verticles to vram
        if(have_verticles)
            glVertexPointer(    3,  to_opengl_type(Format.getAttributeFormat(EVA_POSITION)->type),   0,  MeshBuffer->getBufferData(EVA_POSITION));
        if(have_texcoords)
            glTexCoordPointer(  2,  to_opengl_type(Format.getAttributeFormat(EVA_TEXCOORD)->type),   0,  MeshBuffer->getBufferData(EVA_TEXCOORD));
        if(have_normals)
            glNormalPointer(        to_opengl_type(Format.getAttributeFormat(EVA_NORMAL)->type),     0,  MeshBuffer->getBufferData(EVA_NORMAL));
        if(have_colors)
            glColorPointer(     4,  to_opengl_type(Format.getAttributeFormat(EVA_COLOR)->type),      0,  MeshBuffer->getBufferData(EVA_COLOR));
        //------------------------------------------------------------

        GLenum GLPrimitiveType   = 0;
        u32    VertexInPrimitive = 0;
        //!Convert E_PRIMITIVE_TYPE to GLenum
        to_opengl_primitive((E_PRIMITIVE_TYPE)MeshBuffer->getPrimitiveType(),GLPrimitiveType,VertexInPrimitive);
        //----------------------------------------------
        if(MeshBuffer->getIndicesBufferSize())
            glDrawElements(
				GLPrimitiveType,
				MeshBuffer->getIndicesCount(),
				to_opengl_type(MeshBuffer->getIndicesBufferType()),
				MeshBuffer->getIndicesBufferData());
        else
            glDrawArrays(
				GLPrimitiveType,
				0,
				MeshBuffer->getVertexCount());
        //----------------------------------------------
        PerformanceCounter->register_draw(MeshBuffer->getVertexCount());
        //----------------------------------------------
    }
    else
    {
		if (GLEW_ARB_vertex_array_object)
		{
			MeshBuffer->bind_buffer();

			GLenum GLPrimitiveType = 0;
			u32    VertexInPrimitive = 0;
			//----------------------------------------------
			to_opengl_primitive((E_PRIMITIVE_TYPE)MeshBuffer->getPrimitiveType(), GLPrimitiveType, VertexInPrimitive);

			if (MeshBuffer->getBufferedIndicesCount())
				glDrawElements(
					GLPrimitiveType,
					MeshBuffer->getBufferedIndicesCount(),
					to_opengl_type(MeshBuffer->getIndicesBufferType()),
					0);
			else
				glDrawArrays(
					GLPrimitiveType,
					0,
					MeshBuffer->getBufferedVertexCount());

			MeshBuffer->unbind_buffer();
			//----------------------------------------------
			PerformanceCounter->register_draw(MeshBuffer->getBufferedVertexCount());
			//----------------------------------------------
		}
		else if (GLEW_ARB_vertex_buffer_object)
		{
			MeshBuffer->bind_buffer();

			const SVertexFormat& Format = MeshBuffer->getVertexFormat();
			//------------------------------------------------------------
			bool have_verticles = (Format.getFlags() & EVA_POSITION) != 0;
			bool have_texcoords = (Format.getFlags() & EVA_TEXCOORD) != 0;
			bool have_normals   = (Format.getFlags() & EVA_NORMAL) != 0;
			bool have_colors    = (Format.getFlags() & EVA_COLOR) != 0;

			if (have_verticles == false)
				return;

			u32 BufferShift = 0;
			if (have_verticles)
			{
				glVertexPointer(3, to_opengl_type(Format.getAttributeFormat(EVA_POSITION)->type), 0, (void*)BufferShift);
				BufferShift += MeshBuffer->getBufferSize(EVA_POSITION);
			}
			if (have_normals)
			{
				glNormalPointer(to_opengl_type(Format.getAttributeFormat(EVA_NORMAL)->type), 0, (void*)BufferShift);
				BufferShift += MeshBuffer->getBufferSize(EVA_NORMAL);
			}
			if (have_colors)
			{
				glColorPointer(4, to_opengl_type(Format.getAttributeFormat(EVA_COLOR)->type), 0, (void*)BufferShift);
				BufferShift += MeshBuffer->getBufferSize(EVA_COLOR);
			}
			if (have_texcoords)
			{
				glTexCoordPointer(3, to_opengl_type(Format.getAttributeFormat(EVA_TEXCOORD)->type), 0, (void*)BufferShift);
				BufferShift += MeshBuffer->getBufferSize(EVA_TEXCOORD);
			}
			//----------------------------------------------
			GLenum GLPrimitiveType = 0;
			u32    VertexInPrimitive = 0;

			to_opengl_primitive((E_PRIMITIVE_TYPE)MeshBuffer->getPrimitiveType(), GLPrimitiveType, VertexInPrimitive);

			if (MeshBuffer->getBufferedIndicesCount())
				glDrawElements(
					GLPrimitiveType,
					MeshBuffer->getBufferedIndicesCount(),
					to_opengl_type(MeshBuffer->getIndicesBufferType()),
					0);
			else
				glDrawArrays(
					GLPrimitiveType,
					0,
					MeshBuffer->getBufferedVertexCount());

			MeshBuffer->unbind_buffer();
			//----------------------------------------------
			PerformanceCounter->register_draw(MeshBuffer->getBufferedVertexCount());
			//----------------------------------------------
		}
    }
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::drawPrimitiveList(const SVertex* verticles,u32 VertexCount,E_PRIMITIVE_TYPE PrimitiveType,u32 VertexFormat)
{
    drawIndexedPrimitiveList(NULL,0,verticles,VertexCount,PrimitiveType,VertexFormat);
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::drawIndexedPrimitiveList(const u16* Index,u16 IndexCount,const SVertex* verticles,u32 VertexCount,E_PRIMITIVE_TYPE PrimitiveType,u32 VertexFormat)
{

    if(verticles == NULL)
        return;
    if(VertexCount == 0)
        return;

    //------------------------------------------------------------
    bool have_verticles = (VertexFormat & EVA_POSITION) != 0;
    bool have_texcoords = (VertexFormat & EVA_TEXCOORD) != 0;
    bool have_normals   = (VertexFormat & EVA_NORMAL)   != 0;
    bool have_colors    = (VertexFormat & EVA_COLOR)    != 0;

    //!If no positions in MeshBuffer then nothing to render
    if(have_verticles == false)
        return;

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
        glColorPointer(     4,  GL_UNSIGNED_BYTE,   sizeof(SVertex),    &verticles[0].Color     );
    //------------------------------------------------------------
    GLenum GLPrimitiveType   = 0;
    u32    VertexInPrimitive = 0;
    //!Convert E_PRIMITIVE_TYPE to GLenum
    to_opengl_primitive((E_PRIMITIVE_TYPE)PrimitiveType,GLPrimitiveType,VertexInPrimitive);
    //----------------------------------------------
    if(Index || IndexCount)
        glDrawElements(GLPrimitiveType,IndexCount,GL_UNSIGNED_SHORT,Index);
    else
        glDrawArrays(GLPrimitiveType,0,VertexCount);
    //----------------------------------------------
    PerformanceCounter->register_draw(VertexCount);
    //----------------------------------------------
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::drawArrays(u16 indices_count,u32 vertex_count,const u16* indices,const core::vector3f* verticles,const core::vector2f* texverts,const core::vector3f* normals,const core::color4f* colors,E_PRIMITIVE_TYPE PrimitiveType)
{

    //------------------------------------------------------------
    bool have_verticles = (verticles!=NULL);
    bool have_texcoords = (texverts !=NULL);
    bool have_normals   = (normals  !=NULL);
    bool have_colors    = (colors   !=NULL);

    //!If no positions in MeshBuffer then nothing to render
    if(have_verticles == false)
        return;

    //!Enable/disable client states for drawing
    enable_client_states(have_verticles,have_texcoords,have_normals,have_colors);

    //!Send verticles to vram
    if(have_verticles)
        glVertexPointer(    3,  GL_FLOAT, 0,  verticles);
    if(have_texcoords)
        glTexCoordPointer(  2,  GL_FLOAT, 0,  texverts);
    if(have_normals)
        glNormalPointer(        GL_FLOAT, 0,  normals);
    if(have_colors)
        glColorPointer(     4,  GL_FLOAT, 0,  colors);
    //------------------------------------------------------------
    GLenum GLPrimitiveType   = 0;
    u32    VertexInPrimitive = 0;
    //!Convert E_PRIMITIVE_TYPE to GLenum
    to_opengl_primitive((E_PRIMITIVE_TYPE)PrimitiveType,GLPrimitiveType,VertexInPrimitive);
    //----------------------------------------------
    if(indices != NULL && indices_count != 0)
        glDrawElements(GLPrimitiveType,indices_count,GL_UNSIGNED_SHORT,indices);
    else
        glDrawArrays(GLPrimitiveType,0,vertex_count);
    //----------------------------------------------
    PerformanceCounter->register_draw(vertex_count);
    //----------------------------------------------
}
//-----------------------------------------------------------------------------------------------
bool COpenGLRenderer::isOk()
{
    return !(!noerror || exit);
}
//-----------------------------------------------------------------------------------------------
bool COpenGLRenderer::update()
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
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            LOG_ENGINE_DEBUG("OpenGL error - GL_INVALID_FRAMEBUFFER_OPERATION\n");
            break;
        }
#ifdef NE_DEBUG
        throw;
#endif // NE_DEBUG
    }

    return isOk();
}
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::to_opengl_primitive(E_PRIMITIVE_TYPE engine_primitive,u32& gl_primitive,u32& vertexperprimitive)
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
    case EPT_QUAD:
        gl_primitive = GL_QUADS;
        vertexperprimitive = 4;
        break;
    case EPT_QUAD_STRIP:
        gl_primitive = GL_QUAD_STRIP;
        vertexperprimitive = 2;
        break;
    default:
        gl_primitive = GL_TRIANGLES;
        vertexperprimitive = 3;
        break;
    }
}
//-----------------------------------------------------------------------------------------------
u32 COpenGLRenderer::to_opengl_type(u32 type)
{
    switch(type)
    {
    case NTYPE_s8:
        return GL_BYTE;
    case NTYPE_s16:
        return GL_SHORT;
    case NTYPE_s32:
        return GL_INT;
    case NTYPE_u8:
        return GL_UNSIGNED_BYTE;
    case NTYPE_u16:
        return GL_UNSIGNED_SHORT;
    case NTYPE_u32:
        return GL_UNSIGNED_INT;
    case NTYPE_f32:
        return GL_FLOAT;
    case NTYPE_f64:
        return GL_DOUBLE;
    }
    return 0;
}
//-----------------------------------------------------------------------------------------------
u32 COpenGLRenderer::to_opengl_blendmodes(E_BLENDING_MODE engine_mode)
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
//-----------------------------------------------------------------------------------------------
void COpenGLRenderer::enable_client_states(bool vert,bool tex,bool norm,bool color)
{
    //---------------------------------------------------------MeshBuffer
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
//-----------------------------------------------------------------------------------------------
}
}
