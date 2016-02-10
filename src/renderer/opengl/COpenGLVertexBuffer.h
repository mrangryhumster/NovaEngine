#ifndef COPENGLVERTEXBUFFER_H
#define COPENGLVERTEXBUFFER_H

#include "NovaEngine.h"

#include "IRenderer.h"
#include "CVertexBuffer.h"

#include "log/NLogger.h"

#include "GL/glew.h"

namespace novaengine
{
namespace renderer
{

class COpenGLVertexBuffer : public CVertexBuffer
{
public:
    COpenGLVertexBuffer();
    virtual ~COpenGLVertexBuffer();

};

}
}
#endif // COPENGLVERTEXBUFFER_H
