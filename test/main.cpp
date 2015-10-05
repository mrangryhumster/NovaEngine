#include <vld.h>
#include <stdlib.h>


#include <stdio.h>
#include <cstdio>
#include <wchar.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <windows.h>

#include "NovaEngine.h"

#include "NTime.h"

#include "GL\glew.h"
#include <GL\gl.h>

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

int run()
{

    SEngineConf conf;
    conf.WindowSize = core::dim2u(1920,1080);
    conf.VSync = false;
    conf.StartWindowHidden = false;

    novaengine::INovaEngine* Engine = novaengine::createEngineEx(conf);

    scene::ISceneCamera* Camera = Engine->getSceneManager()->createSceneCamera();
    Camera->setPerspectiveProjectionMatrix(90.f,1920.f / 1080.f,0.1f,3000);
    Camera->RegisterNode();
    Camera->setActive();
    Camera->setPosition(core::vector3f(0,0,4));
    Camera->setTarget(TARGET);
    Camera->release();

    FloatCameraAnimator* animator = new FloatCameraAnimator;

    Engine->getEventManager()->registerEventListener(animator);
    Camera->addAnimator(animator);
    animator->setActive(true);
    animator->scene::ISceneNodeAnimator::release();

    renderer::IStaticMesh* Mesh = Engine->getGeometryManager()->createSphereMesh(4,32);//Engine->getResourceManager()->loadStaticMesh("res//ship.obj");

    u32 FPS = 0;

    Engine->getRenderer()->setRenderState(renderer::ERS_ENABLE_WIREFRAME,true);
    //Engine->getRenderer()->setRenderState(renderer::ERS_ENABLE_CULL_FACE,false);

    novaengine::IPerformanceCounter * EPC = Engine->getPerformanceCounter();
    while(Engine->update())
    {
        //printf("%d %d %f\n",EPC->getVerticesPerFrame(),EPC->getDrawCallsPerFrame(),EPC->getFramesPerSecond());

        Engine->getRenderer()->begin_frame(1,1,core::color4f(0.1f,0.1f,0.4f,1.f));
        Engine->getSceneManager()->animateActiveScene(EPC->getMilisecondsPerFrame());
        Camera->render();

        for(u32 i = 0; i < Mesh->getMeshBuffersCount();i++)
        {
            if(Mesh->getMeshBuffer(i)->getMaterial())
            Engine->getRenderer()->bindMaterial(Mesh->getMeshBuffer(i)->getMaterial());
            Engine->getRenderer()->drawMeshBuffer(Mesh->getMeshBuffer(i));
        }

        Engine->getRenderer()->end_frame();
    }
    Mesh->release();
    closeEngine();
	
    return 0;
}


int main()
{
	run();
    return 0; 
}
