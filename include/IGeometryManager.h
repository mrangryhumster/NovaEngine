#ifndef IGEOMETRYMANAGER_H
#define IGEOMETRYMANAGER_H

#include "Object.h"
#include "NTypes.h"

namespace novaengine
{

namespace renderer
{
class IStaticMesh;
}

class IGeometryManager : public Object
{
    public:

        virtual renderer::IStaticMesh* createCubeMesh() = 0;
        virtual renderer::IStaticMesh* createSphereMesh(float Radius,unsigned int Resolution)  = 0;
        virtual renderer::IStaticMesh* createGridMesh(core::dim2f CellSize,core::dim2u CellCount) = 0;

    protected:
    private:
};

}

#endif // IGEOMETRYMANAGER_H
