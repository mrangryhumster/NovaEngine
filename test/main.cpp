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

#include "gl/glew.h"

#include "COpenGLShaderProgram.h"

using namespace std;
using namespace novaengine;
using namespace renderer;


const char* Default_VertexShader_Source =
"#version 330\n"
"layout (location = 0) in vec3 ne_vertex;\n"
"layout (location = 1) in vec2 ne_texcord;\n"
"uniform mat4  ne_mvp_matrix;\n"
"out vec2 texcord;\n"
"void main() {\n"
"gl_Position = (ne_mvp_matrix) * vec4(ne_vertex,1.0);\n"
"texcord = ne_texcord;\n"
"}\n";

const char* Default_FragmentShader_Source =
"#version 330\n"
"uniform sampler2D ne_texture2de[2];\n"
"in vec2 texcord;\n"
"out vec4 Color;\n"
"void main() {\n"
"  Color = texture(ne_texture2de[0],texcord) - texture(ne_texture2de[1],texcord);\n"
"  Color.a = 1.0;\n"
"}\n";

void debug_output(const char* szFormat, ...)
{
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	OutputDebugString(szBuff);
}

IShaderProgram* Prog = nullptr;

IMaterial* create_material(INovaEngine* Engine)
{

	if (Prog == nullptr)
	{
		Prog = Engine->getRenderer()->createShaderProgram();
		Prog->setVertexShaderSource(Default_VertexShader_Source);
		Prog->setFragmentShaderSource(Default_FragmentShader_Source);
		Prog->build();
	}
	IMaterial* newMaterial = Engine->getResourceManager()->createMaterial();
	newMaterial->setShaderProgram(Prog);
	newMaterial->build();
	//Prog->release();

	return newMaterial;
}

int main()
{
    SEngineConf conf;
    conf.WindowSize.width = 1920;
    conf.WindowSize.height = 1080;

    INovaEngine* Engine = createEngineEx(conf);
    renderer::IRenderer* rndr = Engine->getRenderer();

	ITexture* Texture1 = Engine->getResourceManager()->loadTexture("S:\\F\\git\\res\\m_ship.png");
	ITexture* Texture2 = Engine->getResourceManager()->loadTexture("S:\\F\\git\\res\\m_ship_n.png");
	ITexture* TextureM = Engine->getResourceManager()->loadTexture("S:\\F\\git\\res\\mask.png");

	IStaticMesh* Cube = Engine->getResourceManager()->loadStaticMesh("S:\\F\\git\\res\\m_ship.obj");// Engine->getGeometryManager()->createCubeMesh();

    core::matrixf proj,view,modl;
    proj.buildPerspectiveProjectionMatrix(90, 1920.0 / 1080.0, 0.1, 1000);
	view.buildLookAtMatrix(core::vector3f(-30, 10, 0), core::vector3f(0, 0, 0), core::vector3f(0, 1, 0));
	modl.setTranslate(core::vector3f(0,0,-10));

	rndr->setTransform(proj, EMT_PROJECTION);
	rndr->setTransform(view, EMT_VIEW);
	rndr->setTransform(modl, EMT_MODEL);

	IMaterial* newMaterial1 = create_material(Engine);
	IMaterial* newMaterial2 = create_material(Engine);

	newMaterial1->setTexture(Texture1, 0);
	newMaterial1->setTexture(TextureM, 1);

	newMaterial2->setTexture(Texture2, 0);
	newMaterial2->setTexture(TextureM, 1);

	Prog->bind();

	modl.setTranslate(core::vector3f(0, -4, 20));
	rndr->setTransform(modl, EMT_MODEL);
	newMaterial1->setValue("ne_mvp_matrix", getRenderer()->getTransform(EMT_MVP).getPointer());

	modl.setTranslate(core::vector3f(0, -4, -20));
	rndr->setTransform(modl, EMT_MODEL);
	newMaterial2->setValue("ne_mvp_matrix", getRenderer()->getTransform(EMT_MVP).getPointer());

    //------------------------------------------------------------------------------
	f32 FPS = 0;
	int ctr = 0;
	int frm = 0;
	IPerformanceCounter * EPC = Engine->getPerformanceCounter();
    while (Engine->update())
    {
		if (FPS != EPC->getFramesPerSecond())
		{
			char buf[256];
			sprintf(buf, "fps:%f ctr:%d frm:%d\n", EPC->getFramesPerSecond(),ctr,frm);
			Engine->getWindow()->setTittle(buf);
			FPS = EPC->getFramesPerSecond();
			ctr++;
		}
		frm++;

        rndr->begin_frame();

		for (int i = 0; i < 128; i++)
		{
			newMaterial1->bind();
			Cube->getMeshBuffer(0)->draw();


			newMaterial2->bind();
			Cube->getMeshBuffer(0)->draw();
		}

		rndr->end_frame();

		if (frm > 1000)
			break;
    }
	debug_output("fps:%f ctr:%d frm:%d\n", EPC->getFramesPerSecond(), ctr, frm);

    Cube->release();
    newMaterial1->release();
	newMaterial2->release();
	Prog->release();

	Texture1->release();
	Texture2->release();
	TextureM->release();
    closeEngine();

    return 0;

}
