#ifndef IRENDERER_H
#define IRENDERER_H

#include "Object.h"
#include "IRendererPerformanceCounter.h"
#include "NTypes.h"
#include "ERendererEnums.h"
#include "SVertex.h"
#include "IVertexBuffer.h"
#include "IShaderProgram.h"

#include "IImage.h"
#include "ITexture.h"
#include "IMaterial.h"

namespace novaengine
{
namespace renderer
{
union URenderStateValue
{
    URenderStateValue()
    {}
    URenderStateValue(float value):
        float_value(value)
    {}
    URenderStateValue(s32   value):
        int_value(value)
    {}
    URenderStateValue(bool  value):
        bool_value(value)
    {}
    URenderStateValue(u32   value):
        flags(value)
    {}
    URenderStateValue(u16 value_1,u16 value_2)
    {
        short_value[0] = value_1;
        short_value[1] = value_2;
    }


    f32  float_value;
    int  int_value;
    int  bool_value;
    u16  short_value[2];
    u32  flags;
};

class IRenderer : public Object
{
public:

    virtual u32 getType() = 0;
    //--------------------------------------------------------------------------
    virtual bool update() = 0;
    virtual bool isOk()   = 0;
    //--------------------------------------------------------------------------
    virtual void setVSync(bool) = 0;
    virtual bool getVSync()     = 0;
    //--------------------------------------------------------------------------
    virtual void              setRenderState(u32 flag,URenderStateValue value) = 0;
    virtual URenderStateValue getRenderState(u32 flag)                         = 0;
    //--------------------------------------------------------------------------
    virtual bool isSupported(E_RENDERER_FEATURE ext) = 0;
    virtual s32  QueryRendererFeature(E_RENDERER_FEATURE feature) = 0;
    //--------------------------------------------------------------------------
    virtual void setViewport(core::rectu) = 0;
    virtual core::rectu getViewport()     = 0;
    //--------------------------------------------------------------------------
    virtual void setTransform(const core::matrixf& mat,E_MATRIX_TYPE mtype) = 0;
    virtual const core::matrixf getTransform(E_MATRIX_TYPE mtype) = 0;
    virtual void resetTransform(E_MATRIX_TYPE mtype) = 0;
    //--------------------------------------------------------------------------
    virtual IRendererPerformanceCounter* getRendererPerformanceCounter() = 0;
    //--------------------------------------------------------------------------
    virtual IShaderProgram* GenShaderProgram() = 0;
    //--------------------------------------------------------------------------
    virtual IVertexBuffer* GenVertexBuffer() = 0;
    //--------------------------------------------------------------------------
    virtual ITexture* GenTexture(IImage*,STextureParameters = STextureParameters()) = 0;
    virtual IImage*   GenImage(ITexture*) = 0;
    //--------------------------------------------------------------------------
    virtual void bindTexture(ITexture* texture,u32 texture_unit_id = 0) = 0;
    virtual void bindShaderProgram(IShaderProgram* Shader) = 0;
    //--------------------------------------------------------------------------
    virtual void bindMaterial(IMaterial* material) = 0;
    //--------------------------------------------------------------------------
    virtual void setRenderTarget(ITexture* target,E_RENDER_TARGET_TYPE target_type) = 0;
    //--------------------------------------------------------------------------
    virtual void begin_frame(bool clear_color_buffer = true,bool clear_zbuffer = true,core::color4f clear_color = core::color4f(0,0,0,1)) = 0;
    virtual void end_frame()   = 0;
    //--------------------------------------------------------------------------
    virtual void clear(u32 flag,core::color4f clear_color = core::color4f(0,0,0,1)) = 0;
    virtual void flush() = 0;
    //--------------------------------------------------------------------------
    virtual void drawVertexBuffer(IVertexBuffer*) = 0;

    virtual void drawPrimitiveList       (const SVertex*,u32 VertexCount,E_PRIMITIVE_TYPE,u32 VertexFormat) = 0;
    virtual void drawIndexedPrimitiveList(const u16* Index,u16 IndexCount,const SVertex*,u32 VertexCount,E_PRIMITIVE_TYPE,u32 VertexFormat) = 0;

    virtual void drawArrays(u16 indices_count,u32 vertex_count,const u16* indices,const core::vector3f* verticles,const core::vector2f* texverts,const core::vector3f* normals,const core::color4u* colors,E_PRIMITIVE_TYPE) = 0;

    virtual void drawPoint(core::vector3f Position) = 0;
    virtual void drawLine(core::vector3f Start,core::vector3f End) = 0;
    virtual void drawTriangle(core::vector3f Pos1,core::vector3f Pos2,core::vector3f Pos3) = 0;
    virtual void drawQuad(core::vector3f Pos1,core::vector3f Pos2,core::vector3f Pos3,core::vector3f Pos4) = 0;

    virtual void drawScreenQuad(renderer::ITexture* Texture = NULL,core::color4f color = core::color4f(1,1,1,0)) = 0;
};
}
}


#endif // IRENDERER_H
