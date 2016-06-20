#include <iostream>
#include <time.h>
#include "NovaEngine.h"

using namespace std;
using namespace novaengine;

int main()
{
	SEngineConf Conf;
	Conf.WindowSize.width = 1024;
	Conf.WindowSize.height = 1024;
	INovaEngine* NovaEngine = createEngineEx(Conf);


	renderer::IRenderer* Rndr = NovaEngine->getRenderer();
	window::IWindow* Wndw = NovaEngine->getWindow();


	scene::ISceneCamera* Cam = NovaEngine->getSceneManager()->createSceneCamera();
	Cam->setPerspectiveProjectionMatrix(90.0,1.0,1.0,1000.0);



#define size_g 64
#define size_t 1.0

	std::vector<core::vector3f> grid_pos;
	std::vector<core::color4f> grid_col;
	std::vector<u32> grid_idx;

	for (int x = 0; x < size_g;x++)
		for (int y = 0; y < size_g; y++)
		{
			grid_pos.push_back(core::vector3f((x*size_t)- (size_g*size_t)/2,0, (y*size_t) - (size_g*size_t) /2));
			grid_col.push_back(core::color4f(1.0, 0.0, 1.0, 1.0));
		}

	for (int x = 0; x < size_g-1; x++)
		for (int y = 0; y < size_g-1; y++)
		{
			grid_idx.push_back(((y + 0) + (x + 0) * size_g));
			grid_idx.push_back(((y + 1) + (x + 0) * size_g));
			grid_idx.push_back(((y + 1) + (x + 1) * size_g));
			grid_idx.push_back(((y + 1) + (x + 1) * size_g));
			grid_idx.push_back(((y + 0) + (x + 1) * size_g));
			grid_idx.push_back(((y + 0) + (x + 0) * size_g));

		}


	renderer::IMeshBuffer* MB = NovaEngine->getResourceManager()->createMeshBuffer();

	MB->setPrimitiveType(renderer::EPT_TRIANGLE);
	MB->setVertexFormat(renderer::SVertexFormat(renderer::EVA_POSITION | renderer::EVA_COLOR));
	MB->setIndicesBufferType(NTYPE_u32);

	renderer::IStaticMesh* Sphere = NovaEngine->getGeometryManager()->createSphereMesh(10, 32);

	core::vector3f* rol_backup = nullptr;
	std::vector<u32> idx_rol;
	bool rerol = false;
	u32 mode = 0;
	float time = 0;
	while (NovaEngine->update())
	{
		time += NovaEngine->getPerformanceCounter()->getMilisecondsPerFrame() / 1000.0;

		if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_Q))
		{
			Cam->setPosition(core::vector3f(32, 32, 32));
			Cam->setTarget(core::vector3f(0, 0, 0));
		}
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_W))
		{
			Cam->setPosition(core::vector3f(16, 16, 16));
			Cam->setTarget(core::vector3f(0, 0, 0));
		}
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_E))
		{
			Cam->setPosition(core::vector3f(16, 8, 32));
			Cam->setTarget(core::vector3f(8, 0, 16));
		}

		if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_1))
			mode = 0;
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_2))
			mode = 1;
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_3))
			mode = 2;
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_4))
			mode = 3;
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_5))
			mode = 4;
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_6))
			mode = 5;
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_7))
			mode = 6;
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_8))
			mode = 7;

		if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_R))
			rerol = true;

		if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_A))
			Rndr->setRenderState(renderer::ERS_ENABLE_WIREFRAME, true);
		else if (NovaEngine->getEventManager()->getKeyState(KEY_KEY_S))
			Rndr->setRenderState(renderer::ERS_ENABLE_WIREFRAME, false);




		if (mode == 0)
			for (int x = 0; x < size_g; x++)
				for (int y = 0; y < size_g; y++)
				{
					u16 idx = y + x * size_g;
					float anim_factor = sin(time + grid_pos[idx].x);
					grid_pos[idx].y = anim_factor;
					grid_col[idx].r = (1.0 + anim_factor) / 2.0;
					grid_col[idx].b = 1.0 - (1.0 + anim_factor) / 2.0;
				}
		else if (mode == 1)
			for (int x = 0; x < size_g; x++)
				for (int y = 0; y < size_g; y++)
				{
					u16 idx = y + x * size_g;
					float anim_factor = sin(time - idx);
					grid_pos[idx].y = anim_factor;
					grid_col[idx].r = (1.0 + anim_factor) / 2.0;
					grid_col[idx].b = 1.0 - (1.0 + anim_factor) / 2.0;
				}
		else if (mode == 2)
			for (int x = 0; x < size_g; x++)
				for (int y = 0; y < size_g; y++)
				{
					u16 idx = y + x * size_g;
					float anim_factor = sin(time - core::vector3f(x,0,y).distance_to(core::vector3f(size_g/2,0,size_g/2)));
					grid_pos[idx].y = anim_factor;
					grid_col[idx].r = (1.0 + anim_factor) / 2.0;
					grid_col[idx].b = 1.0 - (1.0 + anim_factor) / 2.0;
				}
		else if (mode == 3)
			for (int x = 0; x < size_g; x++)
				for (int y = 0; y < size_g; y++)
				{
					u16 idx = y + x * size_g;
					float d_Factor = core::vector3f(x, 0, y).distance_to(core::vector3f(size_g / 2, 0, size_g / 2));
					float anim_factor = sin(time - d_Factor);


					if ((d_Factor / 28) > 1.0)
						anim_factor = 0.0;
					else
						anim_factor *= 1.0 - (d_Factor / 28);

					grid_pos[idx].y = anim_factor*2.0;
					grid_col[idx].r = (1.0 + anim_factor) / 2.0;
					grid_col[idx].b = 1.0 - (1.0 + anim_factor) / 2.0;
				}

		else if (mode == 4)
			for (int x = 0; x < size_g; x++)
				for (int y = 0; y < size_g; y++)
				{
					u16 idx = y + x * size_g;
					float d_Factor = core::vector3f(x, 0, y).distance_to(core::vector3f(size_g / 2, 0, size_g / 2));
					float angle = atan2f(y, x) * math::RADTODEG;
					float anim_factor = sin(time - d_Factor + angle);



					if ((d_Factor / 28) > 1.0)
						anim_factor = 0.0;
					else
						anim_factor *= 1.0 - (d_Factor / 28);

					grid_pos[idx].y = anim_factor*2.0;
					grid_col[idx].r = (1.0 + anim_factor) / 2.0;
					grid_col[idx].b = 1.0 - (1.0 + anim_factor) / 2.0;
				}
		else if (mode == 5)
			for (int x = 0; x < size_g; x++)
				for (int y = 0; y < size_g; y++)
				{
					u16 idx = y + x * size_g;
					float d_Factor = core::vector3f(x, 0, y).distance_to(core::vector3f(size_g / 2, 0, size_g / 2))/20.0;
					float anim_factor = tan(time - d_Factor);



					if ((d_Factor / 28) > 1.0)
						anim_factor = 0.0;
					else
						anim_factor *= 1.0 - (d_Factor / 28);

					grid_pos[idx].y = anim_factor*2.0;
					grid_col[idx].r = (1.0 + anim_factor) / 2.0;
					grid_col[idx].b = 1.0 - (1.0 + anim_factor) / 2.0;
				}
		else if (mode == 6)
			for (int x = 0; x < size_g; x++)
				for (int y = 0; y < size_g; y++)
				{
					u16 idx = y + x * size_g;
					float sin_factor = sin(time + grid_pos[idx].x*2.0) / 2.0;
					float cos_factor = cos(time - grid_pos[idx].x/2.0) / 2.0;


					float anim_factor = sin_factor + cos_factor;
					grid_pos[idx].y = anim_factor;
					grid_col[idx].r = (1.0 + anim_factor) / 2.0;
					grid_col[idx].b = 1.0 - (1.0 + anim_factor) / 2.0;
				}
		else if (mode == 7)
		{
			if(rerol)
			{
				Sphere->release();
				Sphere = NovaEngine->getGeometryManager()->createSphereMesh(10, 32);
			}
			u32 vx_count = Sphere->getMeshBuffer(0)->getVertexCount();


			core::vector3f* Pos = (core::vector3f*)Sphere->getMeshBuffer(0)->getBufferData(renderer::EVA_POSITION);


			if (rerol)
			{
				if (rol_backup)
					delete[] rol_backup;
				rol_backup = new core::vector3f[vx_count];

				idx_rol.clear();
				rerol = false;
				for (int i = 0; i < vx_count; i++)
				{
					rol_backup[i] = Pos[i];
					if (rand() % 32 == 0)
						idx_rol.push_back(i);
				}
			}

			float factor = ((sin(time) + 1.0) / 2.0) + 0.5;

			for (int i = 0; i < idx_rol.size(); i++)
			{
				Pos[idx_rol[i]].set(rol_backup[i] * factor);
			}

			Cam->render();
			Rndr->begin_frame(1, 1, core::color4f(0.0f, 0.1f, 0.4f, 1.0f));

			Rndr->drawMeshBuffer(Sphere->getMeshBuffer(0));

			Rndr->end_frame();

			continue;
		}


		Cam->render();
		Rndr->begin_frame(1, 1, core::color4f(0.0f, 0.1f, 0.4f, 1.0f));

		MB->setBufferData(renderer::EVA_POSITION, grid_pos.data(), sizeof(float) * 3 * grid_pos.size());
		MB->setBufferData(renderer::EVA_COLOR, grid_col.data(), sizeof(float) * 4 * grid_col.size());
		MB->setIndicesBufferData(grid_idx.data(), 4 * grid_idx.size());
		Rndr->drawMeshBuffer(MB);

		Rndr->end_frame();
	}

	closeEngine();

	return 0;
}
