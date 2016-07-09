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
const char* v_h_shader =

"varying vec2 v_texCoord;"
"varying vec2 v_blurTexCoords[14];"

"void main()"
"{"
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"    v_texCoord = gl_MultiTexCoord0;"
"    v_blurTexCoords[ 0] = v_texCoord + vec2(-0.014, 0.0);"
"    v_blurTexCoords[ 1] = v_texCoord + vec2(-0.012, 0.0);"
"    v_blurTexCoords[ 2] = v_texCoord + vec2(-0.010, 0.0);"
"    v_blurTexCoords[ 3] = v_texCoord + vec2(-0.008, 0.0);"
"    v_blurTexCoords[ 4] = v_texCoord + vec2(-0.006, 0.0);"
"    v_blurTexCoords[ 5] = v_texCoord + vec2(-0.004, 0.0);"
"    v_blurTexCoords[ 6] = v_texCoord + vec2(-0.002, 0.0);"
"    v_blurTexCoords[ 7] = v_texCoord + vec2( 0.002, 0.0);"
"    v_blurTexCoords[ 8] = v_texCoord + vec2( 0.004, 0.0);"
"    v_blurTexCoords[ 9] = v_texCoord + vec2( 0.006, 0.0);"
"    v_blurTexCoords[10] = v_texCoord + vec2( 0.008, 0.0);"
"    v_blurTexCoords[11] = v_texCoord + vec2( 0.010, 0.0);"
"    v_blurTexCoords[12] = v_texCoord + vec2( 0.012, 0.0);"
"    v_blurTexCoords[13] = v_texCoord + vec2( 0.014, 0.0);"
"}";
const char* v_v_shader =

"varying vec2 v_texCoord;"
"varying vec2 v_blurTexCoords[14];"

"void main()"
"{"
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"    v_texCoord = gl_MultiTexCoord0;"
"    v_blurTexCoords[ 0] = v_texCoord + vec2(0.0, -0.014);"
"    v_blurTexCoords[ 1] = v_texCoord + vec2(0.0, -0.012);"
"    v_blurTexCoords[ 2] = v_texCoord + vec2(0.0, -0.010);"
"    v_blurTexCoords[ 3] = v_texCoord + vec2(0.0, -0.008);"
"    v_blurTexCoords[ 4] = v_texCoord + vec2(0.0, -0.006);"
"    v_blurTexCoords[ 5] = v_texCoord + vec2(0.0, -0.004);"
"    v_blurTexCoords[ 6] = v_texCoord + vec2(0.0, -0.002);"
"    v_blurTexCoords[ 7] = v_texCoord + vec2(0.0,  0.002);"
"    v_blurTexCoords[ 8] = v_texCoord + vec2(0.0,  0.004);"
"    v_blurTexCoords[ 9] = v_texCoord + vec2(0.0,  0.006);"
"    v_blurTexCoords[10] = v_texCoord + vec2(0.0,  0.008);"
"    v_blurTexCoords[11] = v_texCoord + vec2(0.0,  0.010);"
"    v_blurTexCoords[12] = v_texCoord + vec2(0.0,  0.012);"
"    v_blurTexCoords[13] = v_texCoord + vec2(0.0,  0.014);"
"}";

const char* f_shader =
"uniform sampler2D s_texture;"

"varying vec2 v_texCoord;"
"varying vec2 v_blurTexCoords[14];"

"void main()"
"{"
"    gl_FragData[0] = vec4(0.0);"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 0])*0.0044299121055113265;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 1])*0.00895781211794;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 2])*0.0215963866053;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 3])*0.0443683338718;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 4])*0.0776744219933;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 5])*0.115876621105;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 6])*0.147308056121;"
"    gl_FragData[0] += texture2D(s_texture, v_texCoord         )*0.209576912161;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 7])*0.147308056121;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 8])*0.115876621105;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[ 9])*0.0776744219933;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[10])*0.0443683338718;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[11])*0.0215963866053;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[12])*0.00895781211794;"
"    gl_FragData[0] += texture2D(s_texture, v_blurTexCoords[13])*0.0044299121055113265;"
"}";

const char* v_d_shader =

"varying vec2 v_texCoord;"

"void main()"
"{"
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"    v_texCoord = gl_MultiTexCoord0;"
"}";

const char* f_d_shader =
"uniform sampler2D s_texture;"
"varying vec2 v_texCoord;"

"void main()"
"{"
"    gl_FragData[0] = texture2D(s_texture,v_texCoord);"
"    vec3 color = texture2D(s_texture,v_texCoord).rgb;"
"    if(length(color) < 1.0)"
"	 {"
"		gl_FragData[1] = vec4(0.0,0.0,0.0,1.0);"
"	 }"
"	 else"
"		gl_FragData[1] = vec4(color,1.0);"
"}";

const char* f_d2_shader =
"uniform sampler2D s_texture;"
"varying vec2 v_texCoord;"

"void main()"
"{"
"    vec4 color = texture2D(s_texture,v_texCoord);"
"    color.w = length(color.xyz)*2;"
"    color.rgb /= length(color.rgb);"
"    gl_FragData[0] = color;"
"}";


int run()
{

	SEngineConf conf;
	conf.WindowSize = core::dim2u(1920, 1080);
	conf.VSync = false;
	conf.StartWindowHidden = false;

	novaengine::INovaEngine* Engine = novaengine::createEngineEx(conf);

	u32 FPS = 0;
	novaengine::IPerformanceCounter * EPC = Engine->getPerformanceCounter();
	while (Engine->update())
	{
		if (FPS != EPC->getFramesPerSecond())
		{
			char buf[256];
			sprintf(buf, "fps:%f", EPC->getFramesPerSecond());
			Engine->getWindow()->setTittle(buf);
			FPS = EPC->getFramesPerSecond();
		}




	}
	//Mesh->release();

	closeEngine();

	return 0;
}


int main()
{
	run();
	return 0;
}
