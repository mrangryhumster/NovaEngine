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



using namespace std;
using namespace novaengine;
using namespace renderer;


struct S_NMF_Header
{
    uint32_t magic;
    union
    {
        struct
        {
            uint16_t version;
            uint16_t _reserved;
        };
        uint32_t info;
    };
};

struct S_NMF_Description_v0000
{
    uint32_t vertices_count;
    uint32_t indices_count;
    uint32_t contain_flags;
};

int main()
{
	//init engine
	SEngineConf EngineStartupConf;
	EngineStartupConf.WindowSize.width = 1620;
	EngineStartupConf.WindowSize.height = 900;
	EngineStartupConf.LogLevel = log::ELL_DEBUG;

	INovaEngine* Engine = createEngineEx(EngineStartupConf);
	renderer::IRenderer* Renderer = Engine->getRenderer();

	//dev things
	scene::ISceneCamera* DevCamera = Engine->getSceneManager()->createSceneCamera();
	DevCamera->setPerspectiveProjectionMatrix(60.0f, 1.8f, 1.0f, 1000.0f);
	DevCamera->setPosition(core::vector3f(0, 10, 30));

	renderer::ITexture*		DevTexture = Engine->getResourceManager()->loadTexture("..\\..\\res\\m_ship.png");
	renderer::IMaterial*	DevMateial = Engine->getResourceManager()->createMaterial(renderer::ERMF_DEFAULT);
	renderer::IMeshBuffer*	DevModel = Engine->getResourceManager()->loadStaticMesh("..\\..\\res\\monkey.obj.nmf")->getMeshBuffer(0);
	renderer::IMeshBuffer*  DevCube = Engine->getGeometryManager()->createCubeMesh(1.0)->getMeshBuffer(0);
	DevMateial->setTexture(DevTexture, 0);

	cout << DevModel->getVertexCount() << " vertices loaded (" << DevModel->getVertexCount()  / 3 << ")" << endl;
    //-----------------------------
    core::matrixf rotation;
    //Renderer->setRenderState(renderer::ERS_ENABLE_WIREFRAME,true);

	float FPS = 0;
	IPerformanceCounter* EPC = Engine->getPerformanceCounter();
	while (Engine->update())
	{
		if (FPS != EPC->getFramesPerSecond())
		{
			FPS = EPC->getFramesPerSecond();
			printf("%f\n", FPS);
		}

	    float rX = 0.0;
	    float rY = 0.0;
	    float rZ = 0.0;
	    if(Engine->getEventManager()->getKeyState(KEY_KEY_W))
            rZ = -0.01;
        else if(Engine->getEventManager()->getKeyState(KEY_KEY_S))
            rZ =  0.01;
        else if(Engine->getEventManager()->getKeyState(KEY_KEY_A))
            rY = -0.01;
        else if(Engine->getEventManager()->getKeyState(KEY_KEY_D))
            rY =  0.01;

	    rotation.applyRotationDegrees(core::vector3f(0,rY,rZ));
		//Rendering
		Renderer->begin_frame(true, true, core::color4f(0.25f, 0.25f, 0.25f, 1.0f));

		DevCamera->render();
		Renderer->setTransform(rotation,renderer::EMT_MODEL);
		DevMateial->bind();
		DevModel->draw();

		Renderer->end_frame();
		//end of rendering
	}
    cout << "XXXXXXXXXXXXXXXXXXXX____exiting____XXXXXXXXXXXXXXXXXXXXX" << endl;
	closeEngine();

	return 0;

}
