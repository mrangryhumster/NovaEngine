#ifndef COPENGLMeshBuffer_H
#define COPENGLMeshBuffer_H

#include "NovaEngine.h"

#include "IRenderer.h"
#include "CMeshBuffer.h"

#include "NLogger.h"

#include "GL/glew.h"

namespace novaengine
{
namespace renderer
{

class COpenGLMeshBuffer : public CMeshBuffer
{
public:
    COpenGLMeshBuffer();
    virtual ~COpenGLMeshBuffer();

};

}
}
#endif // COPENGLMeshBuffer_H
