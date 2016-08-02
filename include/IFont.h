#ifndef IFONT_H
#define IFONT_H

#include "Resource.h"
#include "ITexture.h"

namespace novaengine
{
namespace gui
{

struct SFontInfo
{
    u32 FontSize;
    u32 LineHeight;
};

struct SGlyph
{
    u16 Character;
    u16 Height;
    u16 Width;

    core::vector3f Vertices[6];
    core::vector2f UVs     [6];
    u16 TexturePage;
    u16 TextureChnl;
};

class IFont : public Resource
{
public:
    virtual SFontInfo     getFontInfo() = 0;

    virtual const SGlyph* getGlyph(u8       character)   = 0;
    virtual const SGlyph* getGlyph(u16 wide_character)   = 0;


    virtual void          setUnknownGlyph(const SGlyph) = 0;
    virtual const SGlyph* getUnknownGlyph()       = 0;


    virtual renderer::ITexture* getTexturePage(u32 page) = 0;

protected:
private:
};

}
}

#endif // IFONT_H
