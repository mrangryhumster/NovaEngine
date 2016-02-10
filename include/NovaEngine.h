#ifndef NOVAENGINE_H
#define NOVAENGINE_H


//Types
#include "NTypes.h"
#include "NMath.h"
#include "NExTypes.h"

#include "types/matrix.h"
#include "types/quaternion.h"
#include "types/vector2.h"
#include "types/vector3.h"
#include "types/color3.h"
#include "types/color4.h"
#include "types/dim2.h"
#include "types/rect.h"

#include "NMath.h"

#include "ILogger.h"
#include "Object.h"
#include "INovaEngine.h"

#include "IWindow.h"

#include "ERendererEnums.h"
#include "IImage.h"
#include "ITexture.h"
#include "IMaterial.h"
#include "IVertexBuffer.h"
#include "IShaderProgram.h"
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
INovaEngine* createEngine();
INovaEngine* createEngineEx(SEngineConf);
void         closeEngine();


INovaEngine*            getNovaEngine();


namespace window
{
IWindow*        getWindow();
}
namespace renderer
{
IRenderer*      getRenderer();
}
namespace log
{
ILogger*           getLogger();
}


}

#endif // NOVAENGINE_H
