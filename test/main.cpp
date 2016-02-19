//#include <vld.h>
#include <stdlib.h>


#include <stdio.h>
#include <cstdio>
#include <wchar.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <windows.h>

#include "NovaEngine.h"
#include "MemoryWatcher.h"

#include "NTime.h"

#include "GL\glew.h"
#include <GL\gl.h>

#include "CMeshBuffer.h"

using namespace std;
using namespace novaengine;

#define SENS_FACTOR 0.2f
#define TARGET core::vector3f(0,0,0)
class FloatCameraAnimator : public scene::ISceneNodeAnimator, public IEventListener
{
public:
    FloatCameraAnimator()
    {
        angle_x = 0;
        angle_y = 0;
        distantion = 5;
        track = 0;
    }
    bool OnEvent(SEvent event)
    {
        if(event.event_type == EET_POINTER_EVENT)
        {
            if(event.pointer.event_type == EPET_BUTTON)
            {
                if(event.pointer.keycode == KEY_MOUSE_LEFT)
                {
                    track = event.pointer.key_state;
                    last_x = event.pointer.x;
                    last_y = event.pointer.y;
                }
            }
            if(event.pointer.event_type == EPET_WHELL)
            {
                if(event.pointer.key_state == EKS_UP)
                    distantion -= 0.5f * SENS_FACTOR * (distantion/2);
                else
                    distantion += 0.5f * SENS_FACTOR * (distantion/2);
            }
            if(track)
            {
                s32 diff_x = event.pointer.x - last_x;
                last_x = event.pointer.x;
                s32 diff_y = event.pointer.y - last_y;
                last_y = event.pointer.y;

                angle_y += diff_x * SENS_FACTOR;
                angle_x += diff_y * SENS_FACTOR;
                if(angle_x > 89)
                    angle_x = 89;
                else if(angle_x < -89)
                    angle_x = -89;
            }
        }
        return true;
    }

    void animate(scene::ISceneNode* AnimatedNode,f32 DeltaTime)
    {
        core::vector3f      Position(0,0,distantion);
        core::matrix<f32>   RotationMatrix;
        RotationMatrix.applyRotationDegrees(core::vector3f(0,angle_y,0));
        RotationMatrix.applyRotationDegrees(core::vector3f(angle_x,0,0));

        RotationMatrix.transform_vector(Position);
        Position.add(TARGET);
        AnimatedNode->setPosition(Position);
    }

private:
    bool track;

    f32 distantion;

    f32 angle_y;
    f32 angle_x;

    s32 last_x;
    s32 last_y;
};
const char* v_shader =

    "varying vec2 v_texCoord;"
    "varying vec2 AOSelector[9];"
    "void main()"
    "{"
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
    "    v_texCoord  = gl_MultiTexCoord0;"
    "    AOSelector[0] = v_texCoord + vec2( 0.01, 0.01);"
    "    AOSelector[1] = v_texCoord + vec2( 0.00, 0.01);"
    "    AOSelector[2] = v_texCoord + vec2(-0.01, 0.01);"
    "    AOSelector[3] = v_texCoord + vec2( 0.01, 0.00);"
    "    AOSelector[4] = v_texCoord + vec2( 0.00, 0.00);"
    "    AOSelector[5] = v_texCoord + vec2(-0.01, 0.00);"
    "    AOSelector[6] = v_texCoord + vec2( 0.01,-0.01);"
    "    AOSelector[7] = v_texCoord + vec2( 0.00,-0.01);"
    "    AOSelector[8] = v_texCoord + vec2(-0.01,-0.01);"
    "}";

const char* f_shader =
    "uniform sampler2D color_texture;"
    "uniform sampler2D depth_texture;"
    "varying vec2 v_texCoord;"
    "varying vec2 AOSelector[9];"
    "void main()"
    "{"
    "    vec4 color = texture2D(color_texture,v_texCoord);"
    "    float curr = texture2D(depth_texture,v_texCoord).r;"
    "    float _avg  = texture2D(depth_texture,AOSelector[0]).r"
    "                + texture2D(depth_texture,AOSelector[1]).r"
    "                + texture2D(depth_texture,AOSelector[2]).r"
    "                + texture2D(depth_texture,AOSelector[3]).r"
    "                + texture2D(depth_texture,AOSelector[4]).r"
    "                + texture2D(depth_texture,AOSelector[5]).r"
    "                + texture2D(depth_texture,AOSelector[6]).r"
    "                + texture2D(depth_texture,AOSelector[7]).r"
    "                + texture2D(depth_texture,AOSelector[8]).r;"
    "    float _mod = 1.0 - (curr - (_avg / 9.0))*8;"
	"    gl_FragData[0] = color * (clamp(_mod,0.5,1.5));"
    "}";

int run()
{

    SEngineConf conf;
    conf.WindowSize = core::dim2u(1920,1080);
    conf.VSync = false;
    conf.StartWindowHidden = false;

    novaengine::INovaEngine* Engine = novaengine::createEngineEx(conf);

    scene::ISceneCamera* Camera = Engine->getSceneManager()->createSceneCamera();
    Camera->setPerspectiveProjectionMatrix(90.f,1920.f/1080.f,1,1000);
    Camera->RegisterNode();
    Camera->setActive();
    Camera->setPosition(core::vector3f(0,0,4));
    Camera->setTarget(TARGET);
    Camera->release();

    FloatCameraAnimator* animator = new FloatCameraAnimator;

    Engine->getEventManager()->registerEventListener(animator);
    Camera->addAnimator(animator);
    animator->setActive(true);
    animator->release();

	renderer::IStaticMesh* Mesh = Engine->getResourceManager()->loadStaticMesh("..\\res\\ssao_mesh.obj");
    //-------------------------------------------------------------------------------

    renderer::ITexture* RTT_Color = Engine->getResourceManager()->createTexture(core::dim2u(1920,1080));
    renderer::ITexture* RTT_Depth = Engine->getResourceManager()->createTexture(core::dim2u(1920,1080),renderer::EPF_DEPTH);

    renderer::IShaderProgram* Shader = Engine->getRenderer()->GenShaderProgram();
    Shader->setVertexShaderSource(v_shader);
    Shader->setFragmentShaderSource(f_shader);
    Shader->compile();

    Shader->bind();
    Shader->bindUniform_TextureUnit(Shader->getUniformLocation("color_texture"),0);
    Shader->bindUniform_TextureUnit(Shader->getUniformLocation("depth_texture"),1);

    //-------------------------------------------------------------------------------
    u32 FPS = 0;

    //Engine->getRenderer()->setRenderState(renderer::ERS_ENABLE_WIREFRAME,true);
    //Engine->getRenderer()->setRenderState(renderer::ERS_ENABLE_CULL_FACE,false);

    novaengine::IPerformanceCounter * EPC = Engine->getPerformanceCounter();
    while(Engine->update())
    {
        char buf[256];
        sprintf(buf,"%f %f %d %d\n",EPC->getFramesPerSecond(),EPC->getMilisecondsPerFrame(),EPC->getDrawCallsPerFrame(),EPC->getVerticesPerFrame());
        Engine->getWindow()->setTittle(buf);
		
        Engine->getRenderer()->begin_frame(1,1,core::color4f(0.1f,0.1f,0.4f,1.f));
       
		Engine->getSceneManager()->animateActiveScene(EPC->getMilisecondsPerFrame());
        
		Camera->render();
		
		
		
        Engine->getRenderer()->bindShaderProgram(nullptr);
        Engine->getRenderer()->setRenderTarget(RTT_Color,renderer::ERTT_COLOR_BUFFER_0);
        Engine->getRenderer()->setRenderTarget(RTT_Depth,renderer::ERTT_DEPTH_BUFFER);
        
		Engine->getRenderer()->setViewport(core::rectu(0,0,1920,1080));
        Engine->getRenderer()->clear(renderer::ECF_COLOR_BUFFER | renderer::ECF_DEPTH_BUFFER);
        
		Engine->getRenderer()->bindTexture(nullptr, 0);
		for(u32 i = 0; i < Mesh->getMeshBuffersCount(); i++)
        {
            if(Mesh->getMeshBuffer(i)->getMaterial())
                Engine->getRenderer()->bindMaterial(Mesh->getMeshBuffer(i)->getMaterial());
            Engine->getRenderer()->drawMeshBuffer(Mesh->getMeshBuffer(i));
        }

        Engine->getRenderer()->bindShaderProgram(Shader);
        Engine->getRenderer()->setRenderTarget(0,0);
        Engine->getRenderer()->setViewport(core::rectu(0,0,1920,1080));
        Engine->getRenderer()->clear(renderer::ECF_DEPTH_BUFFER);

        Engine->getRenderer()->bindTexture(RTT_Depth,1);
        Engine->getRenderer()->drawScreenQuad(RTT_Color);

        Engine->getRenderer()->end_frame();
    }
    Mesh->release();
	Shader->release();
    closeEngine();

    return 0;
}


int main()
{
    run();
    return 0;
}
