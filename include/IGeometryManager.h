#ifndef IGEOMETRYMANAGER_H
#define IGEOMETRYMANAGER_H

#include "Object.h"
#include "NTypes.h"

namespace novaengine
{

namespace renderer
{
class IVertexBuffer;
}

class IGeometryManager : public Object
{
    public:

        virtual renderer::IVertexBuffer* createCubeMesh() = 0;
        virtual renderer::IVertexBuffer* createSphereMesh(float Radius,unsigned int Resolution)  = 0;
        virtual renderer::IVertexBuffer* createGridMesh(core::dim2f CellSize,core::dim2u CellCount) = 0;

    protected:
    private:
};

}

#endif // IGEOMETRYMANAGER_H
