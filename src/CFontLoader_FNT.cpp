
#include "CFontLoader_FNT.h"

#ifdef _NE_INCLUDE_FONT_LOADER_FNT
#include "CFont.h"

//FIX
#include <stdio.h>


namespace novaengine
{

CFontLoader_FNT::CFontLoader_FNT(io::IFileSystem* fs,IResourceManager* rm)
{
    FileSystem = fs;
    ResourceManager = rm;
}
//-----------------------------------------------------------------------------------------------
CFontLoader_FNT::~CFontLoader_FNT()
{
    //dtor
}
//-----------------------------------------------------------------------------------------------
bool CFontLoader_FNT::isSupported(const char* file_extension)
{
    if(strcmp(file_extension,"fnt") == 0)
        return 1;
    return 0;
}
//-----------------------------------------------------------------------------------------------
bool CFontLoader_FNT::isSupported(io::IFile* file)
{
    char buf[9];

    if(file->read(buf,9) != 4) return 0;

    file->seek(file->getLastPos());

    if(strcmp(buf,"info face") == 0)
        return 1;
    return 0;
}
//-----------------------------------------------------------------------------------------------
gui::IFont* CFontLoader_FNT::loadFont(const char* path)
{
    io::IFile* font_file = FileSystem->open_file(path);

    if(!font_file)
        return NULL;

    gui::IFont* font = LoadFNT(font_file);

    font_file->release();

    if(!font)
        return NULL;

    return font;
}
//-----------------------------------------------------------------------------------------------
gui::IFont* CFontLoader_FNT::loadFont(io::IFile* file)
{
    u32 FileStart = file->getPos();
    gui::IFont* font = LoadFNT(file);
    file->seek(FileStart);
    return font;
}
//-----------------------------------------------------------------------------------------------
gui::IFont* CFontLoader_FNT::LoadFNT(io::IFile* file)
{

    char buf[512];
    renderer::ITexture** TexturePages = NULL;
    gui::SGlyph*         Glyphs       = NULL;

    u32 lineHeight,base,scaleW,scaleH,pages,packet,alphaChnl,redChnl,greenChnl,blueChnl,charsCount = 0;
    u32 page_id = 0;
    u32 char_id = 0;
    //---------------------------

    while(file->read_string(buf,512))
    {
        std::string str(buf);
        std::string token = str.substr(0,str.find_first_of(' '));


        if(token.compare("common") == 0)
        {
            sscanf(str.c_str(),
                   "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d packed=%d alphaChnl=%d redChnl=%d greenChnl=%d blueChnl=%d\n",
                   &lineHeight,  &base,  &scaleW,  &scaleH,  &pages,  &packet,  &alphaChnl,  &redChnl,  &greenChnl,  &blueChnl
                  );

            TexturePages = new renderer::ITexture*[pages];

            for(u32 p = 0; p < pages; p++)
                TexturePages[p] = NULL;
        }
        else if(token.compare("page") == 0)
        {
            int s_idx,f_idx;

            s_idx = str.find('\"');

            if(s_idx == -1)
            {
                release_tmp_data(TexturePages,pages,Glyphs,charsCount);
                return NULL;
            }
            f_idx = str.find('\"',s_idx+1);

            if(f_idx == -1)
            {
                release_tmp_data(TexturePages,pages,Glyphs,charsCount);
                return NULL;
            }


            std::string page_file;
            if(file->getSourceType() == io::EFST_NATIVE)
                page_file = std::string(file->getDir()) + str.substr(s_idx+1,f_idx-(s_idx+1));
            else
                page_file = str.substr(s_idx+1,f_idx-(s_idx+1));

            renderer::ITexture* Texture = ResourceManager->loadTexture(page_file.c_str());
            TexturePages[page_id] = Texture;
            page_id++;
        }
        else if(token.compare("chars") == 0)
        {
            sscanf(str.c_str(),"chars count=%d\n",&charsCount);
            Glyphs = new gui::SGlyph[charsCount];

        }
        else if(token.compare("char") == 0)
        {

            int id,x,y,width,height,xoffset,yoffset,xadvance,page,chnl;
            sscanf(str.c_str(),
                   "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d\n",
                   &id,  &x,  &y,  &width,  &height,  &xoffset,  &yoffset,  &xadvance,  &page,  &chnl
                );

            // TODO (Gosha#1#): complete code...


            gui::SGlyph newGlyph;
            newGlyph.Character = id;

            newGlyph.Width  = xadvance;
            newGlyph.Height = 0;    // this

            newGlyph.TexturePage = page;
            newGlyph.TextureChnl = 0; // this

            newGlyph.UVs[0].set((float(x         )/scaleW),  1.f-(float(y         )/scaleH));
            newGlyph.UVs[1].set((float(x         )/scaleW),  1.f-(float(y + height)/scaleH));
            newGlyph.UVs[2].set((float(x + width )/scaleW),  1.f-(float(y + height)/scaleH));
            newGlyph.UVs[3].set((float(x + width )/scaleW),  1.f-(float(y + height)/scaleH));
            newGlyph.UVs[4].set((float(x + width )/scaleW),  1.f-(float(y         )/scaleH));
            newGlyph.UVs[5].set((float(x         )/scaleW),  1.f-(float(y         )/scaleH));


            newGlyph.Vertices[0].set(xoffset         ,  lineHeight - yoffset              ,  0);
            newGlyph.Vertices[1].set(xoffset         ,  lineHeight - (yoffset + height)   ,  0);
            newGlyph.Vertices[2].set(xoffset + width ,  lineHeight - (yoffset + height)   ,  0);
            newGlyph.Vertices[3].set(xoffset + width ,  lineHeight - (yoffset + height)   ,  0);
            newGlyph.Vertices[4].set(xoffset + width ,  lineHeight - yoffset              ,  0);
            newGlyph.Vertices[5].set(xoffset         ,  lineHeight - yoffset              ,  0);



            Glyphs[char_id] = newGlyph;
            char_id++;
        }
        else
        {
            //ops
        }
    }

    gui::SFontInfo FontInfo;
    FontInfo.FontSize   = lineHeight;
    FontInfo.LineHeight = lineHeight;

    gui::CFont* Font = new gui::CFont(FontInfo,Glyphs,charsCount,TexturePages,pages,gui::SGlyph());

    release_tmp_data(TexturePages,pages,Glyphs,charsCount);
    //---------------------------
    LOG_DEBUG("FNT loading complete.\n");
    LOG_DEBUG("Loaded %d texture pages.\n",pages);
    LOG_DEBUG("Loaded %d glyphs.\n",charsCount);
    return Font;
}
//-----------------------------------------------------------------------------------------------
void CFontLoader_FNT::release_tmp_data(renderer::ITexture** TexturePages,u32 TexturePages_count,gui::SGlyph* Glyphs,u32 Glyphs_count)
{
    if(TexturePages != NULL)
    {
        for(u32 p = 0; p < TexturePages_count; p++)
            if(TexturePages[p])
                TexturePages[p]->release();
        delete[] TexturePages;
    }

    if(Glyphs != NULL)
    {
        delete[] Glyphs;
    }
}

}
#endif // _NE_INCLUDE_FONT_LOADER_FNT
