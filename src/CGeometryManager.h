#ifndef CGEOMETRYMANAGER_H
#define CGEOMETRYMANAGER_H

#include "NovaEngine.h"

#include "Object.h"
#include "INovaEngine.h"
#include "IGeometryManager.h"

#include "NLogger.h"


namespace novaengine
{

class CGeometryManager : public IGeometryManager
{
    public:
        CGeometryManager(IResourceManager* ResourceManager_param);
        virtual ~CGeometryManager();

        renderer::IStaticMesh* createCubeMesh(f32 Size);
        renderer::IStaticMesh* createSphereMesh(f32 Radius,u32 Resolution);
        renderer::IStaticMesh* createGridMesh(core::dim2f,core::dim2u);

    protected:
    private:
    IResourceManager* ResourceManager;
};

}
#endif // CGEOMETRYMANAGER_H
