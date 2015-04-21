#include "CResourceManager.h"

#include "CompileConfig.h"

#include "loaders\\CImagePNGLoader.h"

#include "loaders\\CStaticMeshLoader_OBJ.h"
#include "loaders\\CAssimpMeshLoader.h"

#include "NovaEngine.h"

#include "renderer\\CImage.h"
#include "renderer\\CStaticMesh.h"
#include "renderer\\CAnimatedMesh.h"

#include "renderer\\CMaterial.h"

namespace novaengine
{

CResourceManager::CResourceManager(io::IFileSystem* FileSys)
{
    FileSystem = FileSys;

#ifdef NE_INCLUDE_LIBPNG
    registerImageLoader(new CImagePNGLoader(FileSystem));

#endif // INCLUDE_LIBPNG
    registerStaticMeshLoader(new CStaticMeshLoader_OBJ(FileSystem,this));
#ifdef NE_INCLUDE_ASSIMP
    registerStaticMeshLoader(new CAssimpMeshLoader(FileSystem,this));
    registerAnimatedMeshLoader(new CAssimpMeshLoader(FileSystem,this));
#endif // INCLUDE_ASSIMP
}

CResourceManager::~CResourceManager()
{

    u32 ListSize = ImageLoaders.size();
    for(u32 i = 0; i < ListSize; i++)
    {
        if(ImageLoaders[i])
            ImageLoaders[i]->release();
    }

    ListSize = StaticMeshLoaders.size();
    for(u32 i = 0; i < ListSize; i++)
    {
        if(StaticMeshLoaders[i])
            StaticMeshLoaders[i]->release();
    }

    ListSize = AnimatedMeshLoaders.size();
    for(u32 i = 0; i < ListSize; i++)
    {
        if(AnimatedMeshLoaders[i])
            AnimatedMeshLoaders[i]->release();
    }

}

renderer::IImage* CResourceManager::loadImage(const char* ImagePath)
{
    const char* FileExtension = getFileExtension(ImagePath);

    for(u32 irl = 0; irl < ImageLoaders.size(); irl++)
    {
        if(ImageLoaders[irl]->isSupported(FileExtension))
        {
            u32 time = time::getRealTime();
            renderer::IImage* img = ImageLoaders[irl]->LoadImage(ImagePath);
            if(img)
            {
                LOG_INFO("Image loaded : \"%s\" [%d ms]\n",ImagePath,time::getRealTime() - time);
                img->setResourceName(ImagePath);
                return img;
            }
            LOG_ERROR("Image \"%s\" not loaded\n",ImagePath);
            return NULL;
        }
    }
    LOG_ERROR("Unknow format of image \"%s\"[%s]\n",ImagePath,FileExtension);
    return NULL;
}
renderer::ITexture* CResourceManager::loadTexture(const char* TexturePath)
{
    renderer::IImage* Image = loadImage(TexturePath);
    if(!Image)
        return NULL;
    renderer::ITexture* Texture = renderer::getRenderer()->GenTexture(Image);
    Image->release();
    return Texture;
}
void CResourceManager::registerImageLoader(IImageLoader* newLoader)
{
    if(newLoader)
        ImageLoaders.push_back(newLoader);
}
renderer::IStaticMesh*  CResourceManager::loadStaticMesh(const char* MeshPath)
{
    const char* FileExtension = getFileExtension(MeshPath);

    for(u32 irl = 0; irl < StaticMeshLoaders.size(); irl++)
    {
        if(StaticMeshLoaders[irl]->isSupported(FileExtension))
        {
            u32 time = time::getRealTime();
            renderer::IStaticMesh* mesh = StaticMeshLoaders[irl]->LoadStaticMesh(MeshPath);
            if(mesh)
            {
                LOG_INFO("Static mesh loaded : \"%s\" [%d ms]\n",MeshPath,time::getRealTime() - time);
                mesh->setResourceName(MeshPath);
                return mesh;
            }
            LOG_ERROR("Static mesh \"%s\" not loaded\n",MeshPath);
            return NULL;
        }
    }
    LOG_ERROR("Unknow format of static mesh \"%s\"[%s]\n",MeshPath,FileExtension);
    return NULL;
}

void CResourceManager::registerStaticMeshLoader(IStaticMeshLoader* newLoader)
{
    if(newLoader)
        StaticMeshLoaders.push_back(newLoader);
}
renderer::IAnimatedMesh* CResourceManager::loadAnimatedMesh(const char* MeshPath)
{
    const char* FileExtension = getFileExtension(MeshPath);

    for(u32 irl = 0; irl < AnimatedMeshLoaders.size(); irl++)
    {
        if(AnimatedMeshLoaders[irl]->isSupported(FileExtension))
        {
            u32 time = time::getRealTime();
            renderer::IAnimatedMesh* mesh = AnimatedMeshLoaders[irl]->LoadAnimatedMesh(MeshPath);
            if(mesh)
            {
                LOG_INFO("Animated mesh loaded : \"%s\" [%d ms]\n",MeshPath,time::getRealTime() - time);
                mesh->setResourceName(MeshPath);
                return mesh;
            }
            LOG_ERROR("Animated mesh \"%s\" not loaded\n",MeshPath);
            return NULL;
        }
    }
    LOG_ERROR("Unknow format of animated mesh \"%s\"[%s]\n",MeshPath,FileExtension);
    return NULL;
}
renderer::IImage*        CResourceManager::createImage  (core::dim2u resolution,renderer::E_PIXEL_FORMAT pixelformat,u8* pixels)
{
    return new renderer::CImage(pixelformat,resolution,pixels);
}
renderer::ITexture*      CResourceManager::createTexture(core::dim2u resolution,renderer::E_PIXEL_FORMAT pixelformat,u8* pixels)
{
    renderer::IImage* image = createImage(resolution,pixelformat,pixels);
    renderer::ITexture* texture = renderer::getRenderer()->GenTexture(image);
    image->release();
    return texture;
}
renderer::IVertexBuffer* CResourceManager::createVertexBuffer()
{
    return renderer::getRenderer()->GenVertexBuffer();;
}
renderer::IMaterial*     CResourceManager::createMaterial()
{
    return new renderer::CMaterial();
}
renderer::IStaticMesh*   CResourceManager::createStaticMesh()
{
    return new renderer::CStaticMesh();
}
renderer::IAnimatedMesh* CResourceManager::createAnimatedMesh()
{
    return new renderer::CAnimatedMesh();
}
void CResourceManager::registerAnimatedMeshLoader(IAnimatedMeshLoader* newLoader)
{
    if(newLoader)
        AnimatedMeshLoaders.push_back(newLoader);
}
const char* CResourceManager::getFileExtension(const char* FilePath)
{
    u32 FilePath_len = strlen(FilePath);
    for(u32 p = FilePath_len; p > 0; p--)
        if(FilePath[p] == '.')
        {
            if(p == FilePath_len)
                break;
            else
                return &FilePath[p+1];
        }
    return "";
}

}
