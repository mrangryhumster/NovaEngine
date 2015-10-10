#ifndef CFONTLOADER_FNT_H
#define CFONTLOADER_FNT_H

#include "CompileConfig.h"
#ifdef _NE_INCLUDE_STATICMESH_LOADER_OBJ

#include "IFontLoader.h"
#include "IResourceManager.h"
#include "IFileSystem.h"
#include "IFile.h"
#include "NLogger.h"

#include <cstring>
#include <vector>
#include <map>


namespace novaengine
{

class CFontLoader_FNT : public IFontLoader
{
public:
    CFontLoader_FNT(io::IFileSystem* fs,IResourceManager* rm);
    virtual ~CFontLoader_FNT();

    bool isSupported(const char* file_extension);
    bool isSupported(io::IFile* file);

    gui::IFont* loadFont(const char* path);
    gui::IFont* loadFont(io::IFile* file);

    gui::IFont* LoadFNT(io::IFile* file);

protected:
private:
    void release_tmp_data(renderer::ITexture** TexturePages,u32 TexturePages_count,gui::SGlyph* Glyphs,u32 Glyphs_count);

    io::IFileSystem* FileSystem;
    IResourceManager* ResourceManager;
};

}
#endif // _NE_INCLUDE_STATICMESH_LOADER_OBJ
#endif // CFONTLOADER_FNT_H
