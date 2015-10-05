#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

#include "NTime.h"
#include "Log/NLogger.h"

#include "IFileSystem.h"
#include "IResourceManager.h"

#include <vector>

namespace novaengine
{
class CResourceManager : public IResourceManager
{
public:
    CResourceManager(io::IFileSystem*);
    virtual ~CResourceManager();

    renderer::IImage*           loadImage(const char* ImagePath);       //!< Images
    renderer::ITexture*         loadTexture(const char* TexturePath);   //!< Textures
    renderer::IStaticMesh*      loadStaticMesh(const char* MeshPath);   //!< StaticMeshes
    renderer::IAnimatedMesh*    loadAnimatedMesh(const char* MeshPath); //!< AnimatedMesh
    gui::IFont*                 loadFont(const char* ImagePath);        //!< Font

    renderer::IImage*        createImage  (core::dim2u resolution,renderer::E_PIXEL_FORMAT pixelformat = renderer::EPF_R8G8B8A8,u8* pixels = NULL);
    renderer::ITexture*      createTexture(core::dim2u resolution,renderer::E_PIXEL_FORMAT pixelformat = renderer::EPF_R8G8B8A8,u8* pixels = NULL);
    renderer::IMaterial*     createMaterial();
    renderer::IMeshBuffer* createMeshBuffer();
    renderer::IStaticMesh*   createStaticMesh();
    renderer::IAnimatedMesh* createAnimatedMesh();

    void registerImageLoader(IImageLoader* newLoader);
    void registerStaticMeshLoader(IStaticMeshLoader* newLoader);
    void registerAnimatedMeshLoader(IAnimatedMeshLoader* newLoader);
    void registerFontLoader(IFontLoader* newLoader);
    //------

protected:
private:
    inline const char* getFileExtension(const char* FilePath);

    io::IFileSystem*     FileSystem;

    std::vector<IImageLoader*>            ImageLoaders;
    std::vector<IStaticMeshLoader*>       StaticMeshLoaders;
    std::vector<IAnimatedMeshLoader*>     AnimatedMeshLoaders;
    std::vector<IFontLoader*>             FontLoaders;

};

}
#endif // CRESOURCEMANAGER_H
