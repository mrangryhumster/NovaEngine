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
	void OnEvent(SEvent& event)
	{
		if (event.event_type == EET_POINTER_EVENT)
		{
			if (event.pointer.event_type == EPET_BUTTON)
			{
				if (event.pointer.keycode == KEY_MOUSE_LEFT)
				{
					track = event.pointer.key_state;
					last_x = event.pointer.x;
					last_y = event.pointer.y;
				}
			}
			if (event.pointer.event_type == EPET_WHELL)
			{
				if (event.pointer.key_state == EKS_UP)
					distantion -= 0.5f * SENS_FACTOR * (distantion / 2);
				else
					distantion += 0.5f * SENS_FACTOR * (distantion / 2);
			}
			if (track)
			{
				s32 diff_x = event.pointer.x - last_x;
				last_x = event.pointer.x;
				s32 diff_y = event.pointer.y - last_y;
				last_y = event.pointer.y;

				angle_y += diff_x * SENS_FACTOR;
				angle_x += diff_y * SENS_FACTOR;
				if (angle_x > 89)
					angle_x = 89;
				else if (angle_x < -89)
					angle_x = -89;
			}
		}
	}

	void animate(scene::ISceneNode* AnimatedNode, f32 DeltaTime)
	{
		core::vector3f      Position(0, 0, distantion);
		core::matrix<f32>   RotationMatrix;
		RotationMatrix.applyRotationDegrees(core::vector3f(0, angle_y, 0));
		RotationMatrix.applyRotationDegrees(core::vector3f(angle_x, 0, 0));

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
	conf.WindowSize = core::dim2u(1920, 1080);
	conf.VSync = false;
	conf.StartWindowHidden = false;

	novaengine::INovaEngine* Engine = novaengine::createEngineEx(conf);

	u32 FPS = 0;
	novaengine::IPerformanceCounter * EPC = Engine->getPerformanceCounter();

	renderer::IStaticMesh* Mesh = Engine->getGeometryManager()->createCubeMesh();

    core::matrixf proj;
    core::matrixf view;
    core::matrixf modl;

    proj.buildPerspectiveProjectionMatrix(90,1.6666,1.0,1000.0);
    view.buildLookAtMatrix(core::vector3f(0,0,3),core::vector3f(0,0,0),core::vector3f(0,1,0));

    Engine->getRenderer()->setTransform(proj,renderer::EMT_PROJECTION);
    Engine->getRenderer()->setTransform(view,renderer::EMT_VIEW);

    Mesh->getMeshBuffer(0)->setMappingHint(renderer::EMBMH_VBO_STATIC);

	while (Engine->update())
	{
		if (FPS != EPC->getFramesPerSecond())
		{
			char buf[256];
			sprintf(buf, "fps:%f", EPC->getFramesPerSecond());
			Engine->getWindow()->setTittle(buf);
			FPS = EPC->getFramesPerSecond();
		}

        modl.applyRotationDegrees(core::vector3f(0.05,0.075,0.1));
        Engine->getRenderer()->setTransform(modl,renderer::EMT_MODEL);

		Engine->getRenderer()->begin_frame();
		Engine->getRenderer()->drawMeshBuffer(Mesh->getMeshBuffer(0));
		Engine->getRenderer()->end_frame();

	}

	closeEngine();

	return 0;
}


int main()
{
	run();
	return 0;
}
