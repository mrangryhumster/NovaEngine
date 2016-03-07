#include "CResourceManager.h"

#include "CompileConfig.h"

#include "CImageLoader_PNG.h"

#include "CStaticMeshLoader_OBJ.h"

#include "CFontLoader_FNT.h"

#include "CImage.h"
#include "CStaticMesh.h"
#include "CAnimatedMesh.h"
#include "CMaterial.h"

#include "NovaEngine.h"


namespace novaengine
{

CResourceManager::CResourceManager(io::IFileSystem* FileSys)
{
    FileSystem = FileSys;

#ifdef _NE_INCLUDE_LIBPNG
    registerImageLoader(new CImageLoader_PNG(FileSystem));
#endif // INCLUDE_LIBPNG

#ifdef _NE_INCLUDE_STATICMESH_LOADER_OBJ
    registerStaticMeshLoader(new CStaticMeshLoader_OBJ(FileSystem,this));
#endif // _NE_INCLUDE_STATICMESH_LOADER_OBJ

#ifdef _NE_INCLUDE_FONT_LOADER_FNT
    registerFontLoader(new CFontLoader_FNT(FileSystem,this));
#endif // _NE_INCLUDE_FONT_LOADER_FNT
}
//-------------------------------------------------------------------------------------------
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

    ListSize = FontLoaders.size();
    for(u32 i = 0; i < ListSize; i++)
    {
        if(FontLoaders[i])
            FontLoaders[i]->release();
    }
}
//-------------------------------------------------------------------------------------------
renderer::IImage* CResourceManager::loadImage(const char* ImagePath)
{
    const char* FileExtension = getFileExtension(ImagePath);

    for(u32 irl = 0; irl < ImageLoaders.size(); irl++)
    {
        if(ImageLoaders[irl]->isSupported(FileExtension))
		{
            u32 time = time::getRealTime();
            renderer::IImage* img = ImageLoaders[irl]->loadImage(ImagePath);
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
//-------------------------------------------------------------------------------------------
renderer::ITexture* CResourceManager::loadTexture(const char* TexturePath)
{
    renderer::IImage* Image = loadImage(TexturePath);
    if(!Image)
        return NULL;
    renderer::ITexture* Texture = renderer::getRenderer()->createTexture(Image);
    Image->release();
    return Texture;
}
//-------------------------------------------------------------------------------------------
void CResourceManager::registerImageLoader(IImageLoader* newLoader)
{
    if(newLoader)
        ImageLoaders.push_back(newLoader);
}
//-------------------------------------------------------------------------------------------
renderer::IStaticMesh*  CResourceManager::loadStaticMesh(const char* MeshPath)
{
    const char* FileExtension = getFileExtension(MeshPath);

    for(u32 irl = 0; irl < StaticMeshLoaders.size(); irl++)
    {
        if(StaticMeshLoaders[irl]->isSupported(FileExtension))
        {
            u32 time = time::getRealTime();
            renderer::IStaticMesh* mesh = StaticMeshLoaders[irl]->loadStaticMesh(MeshPath);
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
    LOG_ERROR("Unknow format of static mesh \"%s\" [%s]\n",MeshPath,FileExtension);
    return NULL;
}
//-------------------------------------------------------------------------------------------
void CResourceManager::registerStaticMeshLoader(IStaticMeshLoader* newLoader)
{
    if(newLoader)
        StaticMeshLoaders.push_back(newLoader);
}
//-------------------------------------------------------------------------------------------
renderer::IAnimatedMesh* CResourceManager::loadAnimatedMesh(const char* MeshPath)
{
    const char* FileExtension = getFileExtension(MeshPath);

    for(u32 irl = 0; irl < AnimatedMeshLoaders.size(); irl++)
    {
        if(AnimatedMeshLoaders[irl]->isSupported(FileExtension))
        {
            u32 time = time::getRealTime();
            renderer::IAnimatedMesh* mesh = AnimatedMeshLoaders[irl]->loadAnimatedMesh(MeshPath);
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
    LOG_ERROR("Unknow format of animated mesh \"%s\" [%s]\n",MeshPath,FileExtension);
    return NULL;
}
//-------------------------------------------------------------------------------------------
void CResourceManager::registerAnimatedMeshLoader(IAnimatedMeshLoader* newLoader)
{
    if(newLoader)
        AnimatedMeshLoaders.push_back(newLoader);
}
//-------------------------------------------------------------------------------------------
gui::IFont* CResourceManager::loadFont(const char* FontPath)
{
    const char* FileExtension = getFileExtension(FontPath);

    for(u32 irl = 0; irl < FontLoaders.size(); irl++)
    {
        if(FontLoaders[irl]->isSupported(FileExtension))
        {
            u32 time = time::getRealTime();
            gui::IFont* font = FontLoaders[irl]->loadFont(FontPath);
            if(font)
            {
                LOG_INFO("Font loaded : \"%s\" [%d ms]\n",FontPath,time::getRealTime() - time);
                font->setResourceName(FontPath);
                return font;
            }
            LOG_ERROR("Font \"%s\" not loaded\n",FontPath);
            return NULL;
        }
    }
    LOG_ERROR("Unknow format of font \"%s\" [%s]\n",FontPath,FileExtension);
    return NULL;
}
//-------------------------------------------------------------------------------------------
void CResourceManager::registerFontLoader(IFontLoader* newLoader)
{
    if(newLoader)
        FontLoaders.push_back(newLoader);
}
//-------------------------------------------------------------------------------------------
renderer::IImage*        CResourceManager::createImage  (core::dim2u resolution,renderer::E_PIXEL_FORMAT pixelformat,u8* pixels)
{
    return new renderer::CImage(pixelformat,resolution,pixels);
}
//-------------------------------------------------------------------------------------------
renderer::ITexture*      CResourceManager::createTexture(core::dim2u resolution,renderer::E_PIXEL_FORMAT pixelformat,u8* pixels)
{
    renderer::IImage* image = createImage(resolution,pixelformat,pixels);
    renderer::ITexture* texture = renderer::getRenderer()->createTexture(image);
    image->release();
    return texture;
}
//-------------------------------------------------------------------------------------------
renderer::IMeshBuffer* CResourceManager::createMeshBuffer()
{
    return renderer::getRenderer()->createMeshBuffer();;
}
//-------------------------------------------------------------------------------------------
renderer::IMaterial*     CResourceManager::createMaterial()
{
    return new renderer::CMaterial();
}
//-------------------------------------------------------------------------------------------
renderer::IStaticMesh*   CResourceManager::createStaticMesh()
{
    return new renderer::CStaticMesh();
}
//-------------------------------------------------------------------------------------------
renderer::IAnimatedMesh* CResourceManager::createAnimatedMesh()
{
    //REPAIR
    return NULL;//new renderer::CAnimatedMesh();
}
//-------------------------------------------------------------------------------------------
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
