#ifndef IRESOURCEMANAGER_H
#define IRESOURCEMANAGER_H

#include "Object.h"

#include "IImage.h"
#include "ITexture.h"
#include "IImageLoader.h"

#include "IStaticMesh.h"
#include "IStaticMeshLoader.h"

#include "IAnimatedMesh.h"
#include "IAnimatedMeshLoader.h"

#include "IMaterial.h"
#include "IMeshBuffer.h"

#include "IFont.h"
#include "IFontLoader.h"

namespace novaengine
{

class IResourceManager : public Object
{
    public:

            virtual renderer::IImage*           loadImage(const char* ImagePath)        = 0; //!< Images
            virtual renderer::ITexture*         loadTexture(const char* TexturePath)    = 0; //!< Textures
            virtual renderer::IStaticMesh*      loadStaticMesh(const char* MeshPath)    = 0; //!< StaticMeshes
            virtual renderer::IAnimatedMesh*    loadAnimatedMesh(const char* MeshPath)  = 0; //!< AnimatedMesh
            virtual gui::IFont*                 loadFont(const char* ImagePath)         = 0; //!< Font

            virtual renderer::IImage*        createImage  (core::dim2u resolution,renderer::E_PIXEL_FORMAT pixelformat = renderer::EPF_R8G8B8A8,u8* pixels = NULL) = 0;
            virtual renderer::ITexture*      createTexture(core::dim2u resolution,renderer::E_PIXEL_FORMAT pixelformat = renderer::EPF_R8G8B8A8,u8* pixels = NULL) = 0;
            virtual renderer::IMaterial*     createMaterial() = 0;
            virtual renderer::IMeshBuffer* createMeshBuffer() = 0;
            virtual renderer::IStaticMesh*   createStaticMesh() = 0;
            virtual renderer::IAnimatedMesh* createAnimatedMesh() = 0;

            virtual void registerImageLoader(IImageLoader* newLoader)               = 0;
            virtual void registerStaticMeshLoader(IStaticMeshLoader* newLoader)     = 0;
            virtual void registerAnimatedMeshLoader(IAnimatedMeshLoader* newLoader) = 0;
            virtual void registerFontLoader(IFontLoader* newLoader)                 = 0;

    protected:
    private:
};

}
#endif // IRESOURCEMANAGER_H
