#include <iostream>
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

	while (NovaEngine->update())
	{
	
			Rndr->begin_frame(1, 1, core::color4f(0.0f, 0.1f, 0.4f, 1.0f));
			Rndr->end_frame();
	}

	closeEngine();

	return 0;
}
