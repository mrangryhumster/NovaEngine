#ifndef IGEOMETRYMANAGER_H
#define IGEOMETRYMANAGER_H

#include "Object.h"
#include "ne_types.h"

namespace novaengine
{

namespace renderer
{
class IStaticMesh;
}

class IGeometryManager : public Object
{
    public:

        virtual renderer::IStaticMesh* createCubeMesh(f32 Size) = 0;
        virtual renderer::IStaticMesh* createSphereMesh(f32 Radius,u32 Resolution)  = 0;
        virtual renderer::IStaticMesh* createGridMesh(core::dim2f CellSize,core::dim2u CellCount) = 0;

    protected:
    private:
};

}

#endif // IGEOMETRYMANAGER_H
