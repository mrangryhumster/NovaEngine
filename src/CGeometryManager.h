#ifndef CGEOMETRYMANAGER_H
#define CGEOMETRYMANAGER_H

#include "NovaEngine.h"

#include "Object.h"
#include "INovaEngine.h"
#include "IGeometryManager.h"


namespace novaengine
{

class CGeometryManager : public IGeometryManager
{
    public:
        CGeometryManager(IResourceManager* ResourceManager_param);
        virtual ~CGeometryManager();

        renderer::IVertexBuffer* createCubeMesh();
        renderer::IVertexBuffer* createSphereMesh(float Radius,unsigned int Resolution);
        renderer::IVertexBuffer* createGridMesh(core::dim2f,core::dim2u);

    protected:
    private:
    IResourceManager* ResourceManager;
};

}
#endif // CGEOMETRYMANAGER_H
