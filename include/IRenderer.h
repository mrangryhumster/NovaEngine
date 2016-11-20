#ifndef IRENDERER_H
#define IRENDERER_H

#include "Object.h"
#include "ne_types.h"
#include "ERendererEnums.h"
#include "SVertex.h"

#include "IImage.h"
#include "IMaterial.h"

#include "ITexture.h"
#include "IMeshBuffer.h"
#include "IShaderProgram.h"
#include "IRenderTarget.h"

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
    URenderStateValue(core::color4f value)
    {
        color4f[0] = value.r;
        color4f[1] = value.g;
        color4f[2] = value.b;
        color4f[3] = value.a;
    }

    f32  float_value;
    int  int_value;
    int  bool_value;
    u16  short_value[2];
    f32  color4f[4];
    u32  flags;
};

class IRenderer : public Object
{
public:

    virtual u32 getType() = 0;
    //--------------------------------------------------------------------------
    virtual bool update() = 0;
    virtual bool isOk()   = 0;
	virtual bool isReady() = 0;
    //--------------------------------------------------------------------------
    virtual void setVSync(bool) = 0;
    virtual bool getVSync()     = 0;
    //--------------------------------------------------------------------------
    virtual void              setRenderState(u32 flag,URenderStateValue value) = 0;
    virtual URenderStateValue getRenderState(u32 flag)                         = 0;
    //--------------------------------------------------------------------------
    virtual s32  QueryRendererFeature(E_RENDERER_FEATURE feature) = 0;
    //--------------------------------------------------------------------------
    virtual void setViewport(core::rectu) = 0;
    virtual core::rectu getViewport()     = 0;
    //--------------------------------------------------------------------------
    virtual void  setTransform(const core::matrixf& mat,E_MATRIX_TYPE mtype) = 0;
    virtual const core::matrixf& getTransform(E_MATRIX_TYPE mtype) = 0;
    //--------------------------------------------------------------------------
	virtual IRenderTarget*  createRenderTarget() = 0;
	//--------------------------------------------------------------------------
    virtual IShaderProgram* createShaderProgram() = 0;
	virtual IShaderProgram* requestShaderProgram(u32 shader_features = 0) = 0;
    //--------------------------------------------------------------------------
    virtual IMeshBuffer*    createMeshBuffer() = 0;
    //--------------------------------------------------------------------------
    virtual ITexture*       createTexture(IImage*,STextureParameters = STextureParameters()) = 0;
    //--------------------------------------------------------------------------
    virtual void bindTexture(ITexture* texture,u32 texture_unit_id = 0) = 0;
    virtual void bindShaderProgram(IShaderProgram* Shader) = 0;
    //--------------------------------------------------------------------------
    virtual void setRenderTarget(ITexture* target,u32 target_type) = 0;
	virtual void setRenderTarget(IRenderTarget* p_RenderTarget) = 0;
    //--------------------------------------------------------------------------
    virtual void begin_frame(bool clear_color_buffer = true,bool clear_zbuffer = true,core::color4f clear_color = core::color4f(0,0,0,1)) = 0;
    virtual void end_frame()   = 0;
    //--------------------------------------------------------------------------
    virtual void clear(u32 flag,core::color4f clear_color = core::color4f(0,0,0,1)) = 0;
    virtual void flush() = 0;
    //--------------------------------------------------------------------------
    virtual void drawMeshBuffer(IMeshBuffer*) = 0;

    virtual void drawArrays(u16 indices_count,u32 vertex_count,const u16* indices,const core::vector3f* verticles,const core::vector2f* texverts,const core::vector3f* normals,const core::color4f* colors,E_PRIMITIVE_TYPE) = 0;

    virtual void drawPoint(core::vector3f Position) = 0;
    virtual void drawLine(core::vector3f Start,core::vector3f End) = 0;
    virtual void drawTriangle(core::vector3f Pos1,core::vector3f Pos2,core::vector3f Pos3) = 0;
    virtual void drawQuad(core::vector3f Pos1,core::vector3f Pos2,core::vector3f Pos3,core::vector3f Pos4) = 0;

    virtual void drawScreenQuad(renderer::ITexture* Texture = NULL,core::color4f color = core::color4f(1.f,1.f,1.f,1.f)) = 0;
};
}
}


#endif // IRENDERER_H
