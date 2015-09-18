#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <wchar.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <windows.h>

#include "Memorywatcher.h"
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


//--------------------------------------------------------------------------
class Chunk
{
public:
    Chunk()
    {
        memset(data,0,sizeof(int) * 16 * 16 * 16);
    }
    int getVoxel(u32 x,u32 y,u32 z)
    {
        if(x >= 16) return 0;
        if(y >= 16) return 0;
        if(z >= 16) return 0;
        return data[x][y][z];
    }
    void setVoxel(u32 x,u32 y,u32 z,int _data)
    {
        data[x][y][z] = _data;
    }
private:
    int data[16][16][16];
};

class Mesher
{
public:
    renderer::IVertexBuffer* Generate(Chunk* c)
    {
        INovaEngine* Engine = getNovaEngine();

        renderer::IVertexBuffer* VB = Engine->getResourceManager()->createVertexBuffer();
        VB->setVertexFormat(renderer::SVertexFormat(renderer::EVA_POSITION));
        //---------------------------------
        VB->setPrimitiveType(renderer::EPT_TRIANGLE);

        int r = c->getVoxel(0,0,0);
        int r1 = c->getVoxel(1,0,0);

        for(u32 x = 0; x < 16; x++)
            for(u32 y = 0; y < 16; y++)
                for(u32 z = 0; z < 16; z++)
                {
                    int cv = c->getVoxel(x,y,z);

                    if(cv)
                    {
                        if(!c->getVoxel(x-1,y,z))
                        {
                            core::vector3f p[6];
                            p[0].set(x,y,z);
                            p[1].set(x,y,z+1);
                            p[2].set(x,y+1,z+1);
                            p[3] = p[2];
                            p[4].set(x,y+1,z);
                            p[5] = p[0];
                            VB->addBufferData(renderer::EVA_POSITION,p,72);

                        }
                        if(!c->getVoxel(x+1,y,z))
                        {
                            core::vector3f p[6];
                            p[0].set(x+1,y,z+1);
                            p[1].set(x+1,y,z);
                            p[2].set(x+1,y+1,z);
                            p[3] = p[2];
                            p[4].set(x+1,y+1,z+1);
                            p[5] = p[0];
                            VB->addBufferData(renderer::EVA_POSITION,p,72);

                        }
                        if(!c->getVoxel(x,y-1,z))
                        {
                            core::vector3f p[6];
                            p[0].set(x,y,z);
                            p[1].set(x+1,y,z);
                            p[2].set(x+1,y,z+1);
                            p[3] = p[2];
                            p[4].set(x,y,z+1);
                            p[5] = p[0];
                            VB->addBufferData(renderer::EVA_POSITION,p,72);

                        }
                        if(!c->getVoxel(x,y+1,z))
                        {
                            core::vector3f p[6];
                            p[0].set(x,y+1,z);
                            p[1].set(x,y+1,z+1);
                            p[2].set(x+1,y+1,z+1);
                            p[3] = p[2];
                            p[4].set(x+1,y+1,z);
                            p[5] = p[0];
                            VB->addBufferData(renderer::EVA_POSITION,p,72);

                        }
                        if(!c->getVoxel(x,y,z-1))
                        {
                            core::vector3f p[6];
                            p[0].set(x+1,y,z);
                            p[1].set(x,y,z);
                            p[2].set(x,y+1,z);
                            p[3] = p[2];
                            p[4].set(x+1,y+1,z);
                            p[5] = p[0];
                            VB->addBufferData(renderer::EVA_POSITION,p,72);

                        }
                        if(!c->getVoxel(x,y,z+1))
                        {
                            core::vector3f p[6];
                            p[0].set(x,y,z+1);
                            p[1].set(x+1,y,z+1);
                            p[2].set(x+1,y+1,z+1);
                            p[3] = p[2];
                            p[4].set(x,y+1,z+1);
                            p[5] = p[0];
                            VB->addBufferData(renderer::EVA_POSITION,p,72);
                        }
                    }
                    else
                    {

                    }
                }

        //---------------------------------
        return VB;
    }
};
//--------------------------------------------------------------------------

int run()
{

    SEngineConf conf;
    conf.WindowSize = core::dim2u(1920,1080);
    conf.VSync = false;
    conf.StartWindowHidden = false;

    novaengine::INovaEngine* Engine = novaengine::createEngineEx(conf);

    scene::ISceneCamera* Camera = Engine->getSceneManager()->createSceneCamera();
    Camera->setPerspectiveProjectionMatrix(90,1920. / 1080.,0.1,3000);
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

    renderer::IVertexBuffer* VertexBuffer = nullptr;

    Chunk chunk;
    chunk.setVoxel(0,0,0,1);
    chunk.setVoxel(0,1,0,1);
    chunk.setVoxel(0,2,0,1);
    chunk.setVoxel(1,0,0,1);

    Mesher mesher;

    VertexBuffer = mesher.Generate(&chunk);

    renderer::IStaticMesh* Mesh = Engine->getResourceManager()->createStaticMesh();
    Mesh->addMeshUnit(VertexBuffer,NULL);

    scene::ISceneStaticMesh* Node = Engine->getSceneManager()->createSceneStaticMesh(Mesh);
    Node->RegisterNode();
    Node->release();

    Mesh->release();



    u32 FPS = 0;

    Engine->getRenderer()->setRenderState(renderer::ERS_ENABLE_WIREFRAME,true);

    novaengine::IPerformanceCounter * EPC = Engine->getPerformanceCounter();
    while(Engine->update())
    {
        Engine->getRenderer()->begin_frame(1,1,core::color4f(0.1,0.1,0.4,1));
        Engine->getSceneManager()->animateActiveScene(EPC->getMilisecondsPerFrame());
        Engine->getSceneManager()->renderActiveScene();
       Engine->getRenderer()->end_frame();
    }

    closeEngine();
    return 0;
}


int main()
{
    return run();
}
