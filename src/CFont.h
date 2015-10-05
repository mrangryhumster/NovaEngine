#ifndef CFONT_H
#define CFONT_H

#include "IFont.h"

#include <map>

namespace novaengine
{
namespace gui
{

class CFont : public IFont
{
public:
    CFont(SFontInfo p_FontInfo,SGlyph* p_Glyphs,u16 p_Glyphs_count,renderer::ITexture** p_TexturePages,u32 p_TexturePages_count,SGlyph UnknownGlyph)
    {
        for(u16 c = 0; c < p_Glyphs_count; c++)
        {
            Glyphs[p_Glyphs[c].Character] = p_Glyphs[c];
        }

        TexturePages_count = p_TexturePages_count;
        TexturePages = new renderer::ITexture*[p_TexturePages_count];
        for(u32 p = 0; p < p_TexturePages_count; p++)
        {
            TexturePages[p] = p_TexturePages[p];

            if(TexturePages[p])
                TexturePages[p]->capture();
        }
    }
    virtual ~CFont()
    {
        for(u32 p = 0; p < TexturePages_count; p++)
        {
            if(TexturePages[p])
                TexturePages[p]->release();
        }
        delete[] TexturePages;
    }

    SFontInfo getFontInfo()
    {
        return FontInfo;
    }

    const SGlyph* getGlyph(u8 character)
    {
        std::map<u16,SGlyph>::iterator it = Glyphs.find(character);
        if(it == Glyphs.end())
            return &UnknownGlyph;

        return &(*it).second;
    }
    const SGlyph* getGlyph(u16 wide_character)
    {
        std::map<u16,SGlyph>::iterator it = Glyphs.find(wide_character);
        if(it == Glyphs.end())
            return &UnknownGlyph;

        return &(*it).second;
    }

    void setUnknownGlyph(const SGlyph p_UnknownGlyph)
    {
        UnknownGlyph = p_UnknownGlyph;
    }

    const SGlyph* getUnknownGlyph()
    {
        return &UnknownGlyph;
    }


    renderer::ITexture* getTexturePage(u32 page)
    {
        if(page >= TexturePages_count)
            return NULL;
        return TexturePages[page];
    }


protected:
private:

    SFontInfo            FontInfo;

    std::map<u16,SGlyph> Glyphs;
    SGlyph               UnknownGlyph;

    renderer::ITexture** TexturePages;
    u32                  TexturePages_count;
};

}
}


#endif // CFONT_H
