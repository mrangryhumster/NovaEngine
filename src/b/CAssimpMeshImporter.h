#ifndef CASSIMPMESHIMPORTER_H
#define CASSIMPMESHIMPORTER_H

#include "IFileSystem.h"
#include "IStaticMeshImporter.h"
#include "IAnimatedMeshImporter.h"

//#include "libs\assimp\include\assimp\cimport.h"
#include "libs\assimp\include\assimp\Importer.hpp"

#include "libs\assimp\include\assimp\scene.h"
#include "libs\assimp\include\assimp\postprocess.h"


namespace novaengine
{
    /*
class CAssimpIOStream : public Assimp::IOStream
{
public:
    CAssimpIOStream(io::IFile* file)
    {
        NovaEngineIOStream = file;
        NovaEngineIOStream->capture();
    }
    ~CAssimpIOStream()
    {
        this->~IOStream();
        NovaEngineIOStream->release();
    }
    size_t FileSize()
    {
        return NovaEngineIOStream->getSize();
    }

    size_t Read(void* pvBuffer,size_t pSize,size_t pCount)
    {
        size_t Readed = 0;
        for(size_t RC = 0; RC < pCount; RC++)
            Readed = NovaEngineIOStream->read(pvBuffer+Readed,pSize);
        return Readed;
    }

    size_t Write(const void *pvBuffer,size_t pSize,size_t pCount)
    {
        size_t Writed = 0;
        for(size_t WC = 0; WC < pCount; WC++)
            Writed = NovaEngineIOStream->write(pvBuffer,pSize);
        return Writed;
    }

    aiReturn Seek (size_t pOffset, aiOrigin pOrigin)
    {
        if(pOrigin == aiOrigin_END)
        {
            throw pOffset;
        }
        else
        {
            NovaEngineIOStream->seek(pOffset);
        }
    }

    virtual size_t 	Tell () const
    {
        NovaEngineIOStream->getPos();
    }

private:
    io::IFile* NovaEngineIOStream;
};
*/
class CAssimpMeshImporter : public IAnimatedMeshImporter,public IStaticMeshImporter
{
public:

    CAssimpMeshImporter(io::IFileSystem* fs);
    virtual ~CAssimpMeshImporter();

    bool isSupported(const char* file_extension);
    bool isSupported(io::IFile* file);

    renderer::IStaticMesh* LoadStaticMesh(const char* path);
    renderer::IStaticMesh* LoadStaticMesh(io::IFile* file,const char* hint);

    renderer::IAnimatedMesh* LoadAnimatedMesh(const char* path);
    renderer::IAnimatedMesh* LoadAnimatedMesh(io::IFile* file,const char* hint);

    renderer::IStaticMesh*         LoadStaticMesh_assimp(io::IFile* file,const char* Hint);
    renderer::IAnimatedMesh* LoadAnimatedMesh_assimp(io::IFile* file,const char* Hint);

protected:
private:

    int LoadJointsData(aiNode* RootNode,aiMesh* Mesh,renderer::SJoints* Joints);
    int LoadJointRecursively(aiBone* Bone,aiNode* RootNode,aiMesh* Mesh,renderer::SJoints* Joints);
    aiVector3D GetJointAbsolutePosition(aiBone* Bone,aiNode* ){};
    int LoadVertexWeightsData(aiMesh* Mesh,renderer::SJoints* Joints,renderer::IVertexWeightBuffer* Weights);

    io::IFileSystem* FileSystem;
    Assimp::Importer* assimp_importer;
};

}
#endif // CASSIMPMESHIMPORTER_H
