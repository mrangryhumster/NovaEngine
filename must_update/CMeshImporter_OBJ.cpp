#include "CStaticMeshImporter_OBJ.h"

#include "CStaticMesh.h"
#include "CVertexBuffer.h"

#include "createVertexBuffer.h"
#include "createStaticMesh.h"

#include <stdio.h>

namespace novaengine
{

CStaticMeshImporter_OBJ::CStaticMeshImporter_OBJ(io::IFileSystem* fs)
{
    FileSystem = fs;
}
CStaticMeshImporter_OBJ::~CStaticMeshImporter_OBJ()
{

}
bool CStaticMeshImporter_OBJ::isSupported(const char* file_extension)
{
    if(strcmp(file_extension,"obj") == 0)
        return 1;
    return 0;
}
bool CStaticMeshImporter_OBJ::isSupported(io::IFile* file)
{
    return false;//!< Too hard to detect file with this method
}

renderer::IStaticMesh* CStaticMeshImporter_OBJ::LoadMesh(const char* path)
{
    io::IFile* mesh_file = FileSystem->open_file(path);

    if(!mesh_file)
        return NULL;

    renderer::IStaticMesh* mesh = LoadOBJ(mesh_file);

    mesh_file->release();

    if(!mesh)
        return NULL;

    return mesh;
}
renderer::IStaticMesh* CStaticMeshImporter_OBJ::LoadMesh(io::IFile* file,const char* hint)
{
    u32 FileStart = file->getPos();
    renderer::IStaticMesh* mesh = LoadOBJ(file);
    file->seek(FileStart);
    return mesh;
}

renderer::IStaticMesh* CStaticMeshImporter_OBJ::LoadOBJ(io::IFile* file)
{
    bool have_verticles = false;
    bool have_texcoords = false;
    bool have_normals   = false;

    core::array<core::vector3f> Verticles;
    core::array<core::vector2f> TexCoords;
    core::array<core::vector3f> Normals;
    core::array<core::vector3s> Indices;

    core::vector3f tmp_Vertex;
    core::vector2f tmp_Texcoord;
    core::vector3f tmp_Normal;
    core::vector3s tmp_Index;

    char string[256];
    while(file->read_string(string) != 0)
    {

        switch(string[0])
        {
        case '#':
            continue;
        case 'o':
            continue;
        case 'm':
            continue;
        case 'u':
            continue;
        case 'v':
            if(string[1] == ' ')
            {
                have_verticles = true;
                tmp_Vertex.set(0,0,0);
                sscanf(string + 2,"%f %f %f",&tmp_Vertex.x,&tmp_Vertex.y,&tmp_Vertex.z);
                Verticles.push_back(tmp_Vertex);
            }
            else if(string[1] == 't')
            {
                have_texcoords = true;
                tmp_Texcoord.set(0,0);
                sscanf(string + 2,"%f %f",&tmp_Texcoord.x,&tmp_Texcoord.y);
                TexCoords.push_back(tmp_Texcoord);
            }
            else if(string[1] == 'n')
            {
                have_normals = true;
                tmp_Normal.set(0,0,0);
                sscanf(string + 2,"%f %f %f",&tmp_Normal.x,&tmp_Normal.y,&tmp_Normal.z);
                Normals.push_back(tmp_Normal);
            }
            break;
        case 'f':
            core::array<core::vector3s> Face;

            char* tok = strtok(string + 1," \n");
            while(tok != NULL)
            {
                tmp_Index.set(0,0,0);

                if(have_verticles && !have_texcoords && !have_normals)
                    sscanf(tok,"%d",&tmp_Index.x);
                else if(have_verticles && have_texcoords && !have_normals)
                    sscanf(tok,"%d/%d",&tmp_Index.x,&tmp_Index.y);
                else if(have_verticles && !have_texcoords && have_normals)
                    sscanf(tok,"%d//%d",&tmp_Index.x,&tmp_Index.z);
                else if(have_verticles && have_texcoords && have_normals)
                    sscanf(tok,"%d/%d/%d",&tmp_Index.x,&tmp_Index.y,&tmp_Index.z);

                tmp_Index.add(-1);
                Face.push_back(tmp_Index);
                tok = strtok(NULL," \n");
            }
                //Triangulate face
                if(Face.size() > 3)
                {
                    //!< If Face = Quad we use predefined vertex positions (it's faster)
                    //!< Else we split big polygon(more than 4 vertex) on triangles

                    if(Face.size() == 4)
                    {
                        Face.push_back(0);
                        Face.push_back(0);
                        Face[4] = Face[3];
                        Face[3] = Face[2];
                        Face[5] = Face[0];
                    }
                    else
                    {
                        core::array<core::vector3s> TriangulatedFaces;
                        u32 FaceSize = Face.size()-2;
                        for(u32 i = 0; i < FaceSize; i++)
                        {
                            TriangulatedFaces.push_back(Face[0  ]);
                            TriangulatedFaces.push_back(Face[1+i]);
                            TriangulatedFaces.push_back(Face[2+i]);
                        }
                        Face.clear();
                        Face.insert(Face.end(),TriangulatedFaces.begin(),TriangulatedFaces.end());
                    }
                }
                Indices.insert(Indices.end(),Face.begin(),Face.end());
                break;
            }
        }

        renderer::IVertexBuffer* VertexBuffer = renderer::createVertexBuffer();

        u32 IndicesSize = Indices.size();
        for(u32 i = 0; i < IndicesSize; i++)
        {
            renderer::SVertex Vertex;

            if(have_verticles)
                Vertex.Position = Verticles[Indices[i].x];
            if(have_texcoords)
                Vertex.TexCoord = TexCoords[Indices[i].y];
            if(have_normals)
                Vertex.Normal   =   Normals[Indices[i].z];

            VertexBuffer->addVertex(Vertex);
        }

        u32 VertexFormat = renderer::EVF_VERTEX;
        if(have_texcoords)
            VertexFormat |= renderer::EVF_TEXCOORD;
        if(have_normals)
            VertexFormat |= renderer::EVF_NORMAL;


        VertexBuffer->setPrimitiveType(renderer::EPT_TRIANGLES);
        VertexBuffer->setVertexFormat(VertexFormat);
        VertexBuffer->setBufferType(renderer::EVBT_RAWDATA);

        renderer::IStaticMesh* newMesh = renderer::createStaticMesh();
        newMesh->addMeshUnit(VertexBuffer,NULL);

        VertexBuffer->release();

        return newMesh;
    }

}
