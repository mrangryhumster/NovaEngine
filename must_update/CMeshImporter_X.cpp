#include "CMeshImporter_X.h"

namespace novaengine
{

CMeshImporter_X::CMeshImporter_X(io::IFileSystem* fs)
{
    FileSystem = fs;
}

CMeshImporter_X::~CMeshImporter_X()
{
    //dtor
}
bool CMeshImporter_X::isSupported(const char* file_extension)
{
    if(strcmp(file_extension,".x") == 0)
        return 1;
    return 0;
}
bool CMeshImporter_X::isSupported(io::IFile* file)
{

}

renderer::IMesh* CMeshImporter_X::LoadMesh(const char* path)
{
    io::IFile* mesh_file = FileSystem->open_file(path);

    if(!mesh_file)
        return NULL;

    renderer::IMesh* mesh = LoadX(mesh_file);

    mesh_file->release();

    if(!mesh)
        return NULL;

    return mesh;
}
renderer::IMesh* CMeshImporter_X::LoadMesh(io::IFile* file)
{
    u32 FileStart = file->getPos();
    renderer::IMesh* mesh = LoadX(file);
    file->seek(FileStart);
    return mesh;
}

renderer::IMesh* CMeshImporter_X::LoadX(io::IFile* file)
{

}

}
