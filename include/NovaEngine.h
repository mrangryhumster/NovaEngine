#ifndef NOVAENGINE_H
#define NOVAENGINE_H

#include "CompileConfig.h"

#include "ne_types.h"
#include "ne_math.h"

#include "matrix.h"
#include "quaternion.h"
#include "vector2.h"
#include "vector3.h"
#include "color3.h"
#include "color4.h"
#include "dim2.h"
#include "rect.h"

#include "ILogger.h"
#include "Object.h"
#include "INovaEngine.h"

#include "IWindow.h"

#include "ERendererEnums.h"
#include "IImage.h"
#include "ITexture.h"
#include "IMaterial.h"
#include "IMeshBuffer.h"
#include "IShaderProgram.h"
#include "IRenderTarget.h"
#include "IRenderer.h"

#include "IFileSystem.h"
#include "IFile.h"

#include "IResourceManager.h"


#include "IGeometryManager.h"

#include "ISceneManager.h"
#include "IScene.h"
#include "ISceneNode.h"
#include "IScene2DNode.h"
#include "ISceneCamera.h"
#include "ISceneSprite.h"
#include "ISceneStaticMesh.h"

#include "SEngineConf.h"
#include "SPixelFormat.h"
#include "STextureParameters.h"
#include "SPrimitive.h"

#include "EKeycodes.h"
#include "ERendererEnums.h"


// TODO (Gosha#1#): Rewrite, totally, AnimatedMesh...
// TODO (Gosha#1#): Rewrite, totally, whole engine


namespace novaengine
{
_NOVAENGINE_API INovaEngine* createEngine();
_NOVAENGINE_API INovaEngine* createEngineEx(SEngineConf);
_NOVAENGINE_API void         closeEngine();


_NOVAENGINE_API  INovaEngine*    getNovaEngine();

namespace window
{
_NOVAENGINE_API  IWindow*        getWindow();
}
namespace renderer
{
_NOVAENGINE_API  IRenderer*      getRenderer();
}
namespace log
{
_NOVAENGINE_API  ILogger*        getLogger();
}


}

#endif // NOVAENGINE_H
