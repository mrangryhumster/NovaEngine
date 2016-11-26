//#include <vld.h>
#include <stdlib.h>


#include <stdio.h>
#include <cstdio>
#include <wchar.h>
#include <iostream>
#include <fstream>
#include <time.h>


#include "NovaEngine.h"


#include "NTime.h"



using namespace std;
using namespace novaengine;
using namespace renderer;


int main()
{
	//init engine
	SEngineConf EngineStartupConf;
	EngineStartupConf.WindowSize.width = 640;
	EngineStartupConf.WindowSize.height = 480;
	EngineStartupConf.LogLevel = log::ELL_DEBUG;

	INovaEngine* Engine = createEngineEx(EngineStartupConf);
	renderer::IRenderer* Renderer = Engine->getRenderer();

	printf("Init.eng.d\n");
	//dev things
	scene::ISceneCamera* DevCamera = Engine->getSceneManager()->createSceneCamera();
	DevCamera->setPerspectiveProjectionMatrix(90.0f, 640.0/480.0, 1.0f, 1000.0f);
	DevCamera->setPosition(core::vector3f(5, 5, 5));
    printf("Init.cam.d\n");

	renderer::IMaterial*	DevMateial = Engine->getResourceManager()->createMaterial(renderer::ERMF_DEFAULT);
	printf("Init.res.mat.d\n");
	renderer::IStaticMesh*	DevCube = Engine->getGeometryManager()->createCubeMesh(1.0);
    printf("Init.res.d\n");

	//-----------------------------
	float FPS = 0;
	IPerformanceCounter* EPC = Engine->getPerformanceCounter();
	while (Engine->update())
	{
		if (FPS != EPC->getFramesPerSecond())
		{
			FPS = EPC->getFramesPerSecond();
			printf("%f\n", FPS);
		}

		//Rendering
		Renderer->begin_frame(true, true, core::color4f(0.25f, 0.25f, 0.25f, 1.0f));

		DevCamera->render();
		DevMateial->bind();
		DevCube->getMeshBuffer(0)->draw();

		Renderer->end_frame();

		Engine->getWindow()->getWindowInternalVariable("swap");
	}
	closeEngine();

	return 0;

}
