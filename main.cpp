

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

#include "renderer\CMaterial.h"

using namespace std;
using namespace novaengine;
class CamAnimator : public scene::ISceneNodeAnimator
{
public:
    void setActive()
    {

    }
    bool isActive()
    {
        return true;
    }

    u32  getID()
    {
        return 1;
    }
    void animate(scene::ISceneNode* AnimatedNode,f32 DeltaTime)
    {
        core::vector3f Position = AnimatedNode->getPosition();

        counter += (DeltaTime / 1000)*0.2;
        Position.x = sin(counter)*40;
        Position.y = 20;
        Position.z = cos(counter)*40;
    AnimatedNode->setPosition(Position);
    }
    float counter;
};

int run()
{
    SEngineConf conf;

    novaengine::INovaEngine* Engine = novaengine::createEngineEx(conf);

    scene::ISceneCamera* Camera = Engine->getSceneManager()->createSceneCamera();
    Camera->setPerspectiveProjectionMatrix(90,1920. / 1080.,1,1000);
    Camera->RegisterNode();
    Camera->setActive();
    Camera->setPosition(core::vector3f(0,20,-4));
    Camera->setTarget(core::vector3f(0,0,0));
    CamAnimator* an = new CamAnimator();
    Camera->addAnimator(an);
    an->release();

    renderer::IVertexBuffer* Cube_vb = Engine->getGeometryManager()->createCubeMesh();
    renderer::IStaticMesh* Cube_mesh = Engine->getResourceManager()->createStaticMesh();
    Cube_mesh->addMeshUnit(Cube_vb,NULL);

    for(int z = -80; z < 80; z+=4)
        for(int x = -80; x < 80; x+=4)
        {
            scene::ISceneStaticMesh* Cube_node = Engine->getSceneManager()->createSceneStaticMesh(Cube_mesh);
            Cube_node->RegisterNode();
            Cube_node->setPosition(core::vector3f(x,0,z));
            Cube_node->release();
        }


    Camera->release();
    Cube_vb->release();
    Cube_mesh->release();

    u32 FPS = 0;
    u32 start_time = clock();
    novaengine::renderer::IRendererPerformanceCounter* RPC = Engine->getRenderer()->getRendererPerformanceCounter();
    while(Engine->update())
    {
        if(FPS != RPC->getFramePerSecond())
        {
            FPS = RPC->getFramePerSecond();
        }

        Engine->getRenderer()->begin_frame();
        Engine->getSceneManager()->renderActiveScene();
        Engine->getSceneManager()->animateActiveScene(RPC->getMilisecondPerFrame());
        Engine->getRenderer()->end_frame();
    }

    closeEngine();
}
int main()
{
run();
}
