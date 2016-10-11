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

//shaders

const char* Default_VRTX =
"#version 330\n"
"layout (location = 0) in vec3 ne_vertex;\n"
"uniform mat4  ne_mvpmatrix;\n"
"void main()\n"
"{\n"
"gl_Position = ne_mvpmatrix * vec4(ne_vertex,1.0);\n"
"}\n";

const char* White_FRAG =
"#version 330\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
"  outColor = vec4(1.0,1.0,1.0,1.0);\n"
"}\n";
const char* Black_FRAG =
"#version 330\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
"  outColor = vec4(0.0,0.0,0.0,1.0);\n"
"}\n";

const char* Scat_VRTX =
"#version 330\n"
"layout (location = 0) in vec3 ne_vertex;\n"
"layout (location = 1) in vec2 ne_tex;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(ne_vertex,1.0);\n"
"TexCoord = ne_tex;\n"
"}\n";
const char* Scat_FRAG =
"#version 330\n"
"\n"
"    uniform float exposure;\n"
"    uniform float decay;\n"
"    uniform float density;\n"
"    uniform float weight;\n"
"    uniform vec2 lightPositionOnScreen;\n"
"    uniform sampler2D firstPass;\n"
"    const int NUM_SAMPLES = 100 ;\n"
"	 in vec2 TexCoord;\n"
"	 out vec4 FragColor;\n"
"\n"
"    void main()\n"
"    {\t\n"
"    \tvec2 deltaTextCoord = vec2( TexCoord.st - lightPositionOnScreen.xy );\n"
"    \tvec2 textCoo = TexCoord.st;\n"
"    \tdeltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;\n"
"    \tfloat illuminationDecay = 1.0;\n"
"\t\n"
"\t\n"
"    \tfor(int i=0; i < NUM_SAMPLES ; i++)\n"
"        {\n"
"                 textCoo -= deltaTextCoord;\n"
"                 vec4 sample = texture2D(firstPass, textCoo );\n"
"\t\t\t\n"
"                 sample *= illuminationDecay * weight;\n"
"\n"
"                 FragColor += sample;\n"
"\n"
"                 illuminationDecay *= decay;\n"
"         }\n"
"         FragColor *= exposure;\n"
"    }";

#include "GL/glew.h"

//-------
int main()
{
	SEngineConf conf;
	conf.WindowSize.width = 1920;
	conf.WindowSize.height = 1080;

	INovaEngine* Engine = createEngineEx(conf);
	renderer::IRenderer* rndr = Engine->getRenderer();


	scene::ISceneCamera* Camera = Engine->getSceneManager()->createSceneCamera();
	Camera->setPerspectiveProjectionMatrix(120, 1920.0 / 1080.0, 1.0, 1000.0);
	Camera->setPosition(core::vector3f(30, 0, 0));
	Camera->setTarget(core::vector3f(-100, 0, 0));

	scene::ISceneCamera* Camera2 = Engine->getSceneManager()->createSceneCamera();
	Camera2->setOrthographicProjectionMatrix(-1.0, 1.0, -1.0, 1.0, -1000, 1000);

	IShaderProgram* VLProg_w = Engine->getRenderer()->createShaderProgram();
	IShaderProgram* VLProg_b = Engine->getRenderer()->createShaderProgram();

	VLProg_w->setVertexShaderSource(Default_VRTX);
	VLProg_w->setFragmentShaderSource(White_FRAG);
	VLProg_b->setVertexShaderSource(Default_VRTX);
	VLProg_b->setFragmentShaderSource(Black_FRAG);
	VLProg_w->build();
	VLProg_b->build();



	IStaticMesh* Cube_Mesh = Engine->getGeometryManager()->createCubeMesh();
	IStaticMesh* Sphere_Mesh = Engine->getGeometryManager()->createSphereMesh(7.0, 16);

	core::matrixf Sphere_Transform, sqt;
	Sphere_Transform.setTranslate(core::vector3f(-100, 0, 0));


	core::matrixf Ship_Transform[2];
	IMaterial* Ship_Material = Engine->getResourceManager()->createMaterial();
	ITexture* Ship_Texture = Engine->getResourceManager()->loadTexture("S:\\F\\git\\res\\m_ship.png");
	IStaticMesh* Ship_Mesh = Engine->getResourceManager()->loadStaticMesh("S:\\F\\git\\res\\m_ship.obj");

	Ship_Material->setTexture(Ship_Texture, 0);
	Ship_Transform[0].setTranslate(core::vector3f(-20, 5, -60));
	Ship_Transform[0].setRotationDegrees(core::vector3f(-70, 0, 0));
	Ship_Transform[1].setTranslate(core::vector3f(0, -5, 20));
	Ship_Transform[1].setRotationDegrees(core::vector3f(20, 0, 0));

	//----light
	ITexture* DepthBuffer = Engine->getResourceManager()->createTexture(core::dim2u(1920, 1080), EPF_DEPTH);
	ITexture* ColorBuffer = Engine->getResourceManager()->createTexture(core::dim2u(1920, 1080));
	IRenderTarget* RenderTarget = Engine->getRenderer()->createRenderTarget();
	RenderTarget->setTexture(ColorBuffer, ERTT_COLOR_BUFFER_0);
	RenderTarget->setTexture(DepthBuffer, ERTT_DEPTH_BUFFER);

	ITexture* DepthBuffer2 = Engine->getResourceManager()->createTexture(core::dim2u(1920, 1080), EPF_DEPTH);
	ITexture* ColorBuffer2 = Engine->getResourceManager()->createTexture(core::dim2u(1920, 1080));
	IRenderTarget* RenderTarget2 = Engine->getRenderer()->createRenderTarget();
	RenderTarget2->setTexture(ColorBuffer2, ERTT_COLOR_BUFFER_0);
	RenderTarget2->setTexture(DepthBuffer2, ERTT_DEPTH_BUFFER);

	IShaderProgram* VLProg = Engine->getRenderer()->createShaderProgram();
	VLProg->setVertexShaderSource(Scat_VRTX);
	VLProg->setFragmentShaderSource(Scat_FRAG);
	VLProg->build();
	
	float exposure(0.0034);
	float decay(1.0);
	float density(0.94);
	float weight(5.64);

	core::vector2f lightPositionOnScreen(0.5, 0.5);
	int firstPass = 5;


	VLProg->bind();
	VLProg->setUniform("exposure", ESUT_FLOAT1, 1, &exposure);
	VLProg->setUniform("decay", ESUT_FLOAT1, 1, &decay);
	VLProg->setUniform("density", ESUT_FLOAT1, 1, &density);
	VLProg->setUniform("weight", ESUT_FLOAT1, 1, &weight);
	VLProg->setUniform("lightPositionOnScreen", ESUT_FLOAT2, 1, &lightPositionOnScreen.x);
	VLProg->setUniform("firstPass", ESUT_TEXTURE_2D, 1, &firstPass);




	core::matrixf dummy;
	//------------------------------------------------------------------------------
	f32 FPS = 0;
	int ctr = 0;
	int frm = 0;
	IPerformanceCounter * EPC = Engine->getPerformanceCounter();
	while (Engine->update())
	{
		//Camera->setPosition(core::vector3f(sin(frm/200.0)*100, 0, cos(frm/200.0)*100));
		if (FPS != EPC->getFramesPerSecond())
		{
			char buf[256];
			sprintf(buf, "fps:%f ctr:%d frm:%d\n", EPC->getFramesPerSecond(), ctr, frm);
			Engine->getWindow()->setTittle(buf);
			FPS = EPC->getFramesPerSecond();
			ctr++;
		}
		frm++;

		rndr->begin_frame();
		rndr->setRenderTarget(0);

		Camera->render();
		rndr->setViewport(core::rectu(0, 0, 1920, 1080));
		//------------------------------------------------
		rndr->setRenderTarget(RenderTarget);

		rndr->clear(ECF_COLOR_BUFFER | ECF_DEPTH_BUFFER,core::color4f(0.00,0.00,0.05,1.0));

		VLProg_b->bind();
		rndr->setTransform(Ship_Transform[0], EMT_MODEL);
		Ship_Mesh->getMeshBuffer(0)->draw();
		rndr->setTransform(Ship_Transform[1], EMT_MODEL);
		Ship_Mesh->getMeshBuffer(0)->draw();

		rndr->setRenderState(ERS_DEPTH_TEST_MODE, EDTM_ALWAYS);

		VLProg_w->bind();
		rndr->setTransform(Sphere_Transform, EMT_MODEL);
		Sphere_Mesh->getMeshBuffer(0)->draw();
		rndr->setRenderState(ERS_DEPTH_TEST_MODE, EDTM_LEQUAL);

		//------------------------------------------------
		rndr->setRenderTarget(RenderTarget2);
		rndr->clear(ECF_COLOR_BUFFER | ECF_DEPTH_BUFFER, core::color4f(0.0, 0.0, 0.0, 1.0));

		Ship_Material->bind();
		rndr->setTransform(Ship_Transform[0], EMT_MODEL);
		Ship_Mesh->getMeshBuffer(0)->draw();
		rndr->setTransform(Ship_Transform[1], EMT_MODEL);
		Ship_Mesh->getMeshBuffer(0)->draw();

		rndr->setRenderState(ERS_BLENDING_MODE, URenderStateValue(EBM_SRC_ALPHA,EBM_ONE));
		rndr->bindTexture(ColorBuffer, 5);
		VLProg->bind();
		Camera2->render();
		rndr->setTransform(dummy, EMT_MODEL);

		Cube_Mesh->getMeshBuffer(0)->draw();

		//------------------------------------------------
		rndr->setRenderState(ERS_BLENDING_MODE, URenderStateValue(EBM_SRC_ALPHA, EBM_ONE_MINUS_SRC_ALPHA));
		rndr->setRenderTarget(0);
		rndr->clear(ECF_COLOR_BUFFER | ECF_DEPTH_BUFFER, core::color4f(0.0, 0.0, 0.0, 1.0));

		Ship_Material->bind();
		rndr->bindTexture(ColorBuffer2, 0);
		Camera2->render();
		rndr->setTransform(dummy, EMT_MODEL);
		Cube_Mesh->getMeshBuffer(0)->draw();

		rndr->end_frame();

		//if (frm > 30000)
		//break;
	}
	Camera->release();

	closeEngine();

	return 0;

}
