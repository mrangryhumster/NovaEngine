#ifndef SPRIMITIVE_H
#define SPRIMITIVE_H


#include "NTypes.h"
#include "NExTypes.h"
#include "SVertex.h"

namespace novaengine
{
namespace renderer
{

//------------------------------------------
enum E_PRIMITIVE_TYPE
{
	EPT_UNDEFINED           = 0x0,
	EPT_POINT               = 0x1,
	EPT_LINE                = 0x2,
	EPT_LINE_STRIP          = 0x3,
	EPT_TRIANGLE            = 0x4,
	EPT_TRIANGLE_STRIP      = 0x5,
	EPT_QUAD                = 0x6,
	EPT_QUAD_STRIP          = 0x7,
};
//------------------------------------------
struct SPrimitive
{
	virtual SVertex* getVertices()
	{
		return NULL;
	};
	virtual u32 getVertexCount()
	{
		return 0;
	};
	virtual u32 getPolygonType()
	{
		return EPT_UNDEFINED;
	};

protected:
	SPrimitive() {};
};
//------------------------------------------
struct SPrimitivePoint : public SPrimitive
{
	SPrimitivePoint()
	{}
	SPrimitivePoint(SVertex* vx_ptr,u32 offset = 0)
	{
        point = vx_ptr[offset];
	}

	SVertex* getVertices()
	{
		return &point;
	}
	u32 getVertexCount()
	{
		return 1;
	}
	u32 getPolygonType()
	{
		return EPT_POINT;
	}
	SVertex point;
};
//------------------------------------------
struct SPrimitiveLine : public SPrimitive
{
	SPrimitiveLine()
	{}
	SPrimitiveLine(SVertex* vx_ptr,u32 offset = 0)
	{
        memcpy(points,&vx_ptr[offset],2);
	}

	SVertex* getVertices()
	{
		return points;
	}
	u32 getVertexCount()
	{
		return 2;
	}
	u32 getPolygonType()
	{
		return EPT_LINE;
	}
	SVertex points[2];
};
//------------------------------------------
struct SPrimitiveTriangle : public SPrimitive
{
	SPrimitiveTriangle()
	{}
	SPrimitiveTriangle(SVertex* vx_ptr,u32 offset = 0)
	{
        memcpy(points,&vx_ptr[offset],3);
	}
	SVertex* getVertices()
	{
		return points;
	}
	u32 getVertexCount()
	{
		return 3;
	}
	u32 getPolygonType()
	{
		return EPT_TRIANGLE;
	}
	SVertex points[3];
};
//------------------------------------------
struct SPrimitiveQuad : public SPrimitive
{
	SPrimitiveQuad()
	{}
	SPrimitiveQuad(SVertex* vx_ptr,u32 offset = 0)
	{
        memcpy(points,&vx_ptr[offset],4);
	}
	SVertex* getVertices()
	{
		return points;
	}
	u32 getVertexCount()
	{
		return 3;
	}
	u32 getPolygonType()
	{
		return EPT_QUAD;
	}
	SVertex points[4];
};
//------------------------------------------

}
}
#endif // SPRIMITIVE_H
