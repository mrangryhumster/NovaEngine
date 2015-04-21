#include "CStaticMeshLoader_OBJ.h"

#include "CStaticMesh.h"
#include "CVertexBuffer.h"

#include <stdio.h>


namespace novaengine
{

CStaticMeshLoader_OBJ::CStaticMeshLoader_OBJ(io::IFileSystem* fs,IResourceManager* rm)
{
    FileSystem      = fs;
    ResourceManager = rm;
}
CStaticMeshLoader_OBJ::~CStaticMeshLoader_OBJ()
{

}
bool CStaticMeshLoader_OBJ::isSupported(const char* file_extension)
{
    if(strcmp(file_extension,"obj") == 0)
        return 1;
    return 0;
}
bool CStaticMeshLoader_OBJ::isSupported(io::IFile* file)
{
    return false;//!< Too hard to detect file with this method
}

renderer::IStaticMesh* CStaticMeshLoader_OBJ::LoadStaticMesh(const char* path)
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
renderer::IStaticMesh* CStaticMeshLoader_OBJ::LoadStaticMesh(io::IFile* file,const char* hint)
{
    u32 FileStart = file->getPos();
    renderer::IStaticMesh* mesh = LoadOBJ(file);
    file->seek(FileStart);
    return mesh;
}

renderer::IStaticMesh* CStaticMeshLoader_OBJ::LoadOBJ(io::IFile* file)
{
    bool have_texcoords = false;
    bool have_normals   = false;

    std::vector<core::vector3f> Verticles;
    std::vector<core::vector2f> TexCoords;
    std::vector<core::vector3f> Normals;

    std::map<std::string,SObjMtl> ObjMaterials;

    renderer::IStaticMesh*   Mesh         = ResourceManager->createStaticMesh();
    renderer::IVertexBuffer* VertexBuffer = ResourceManager->createVertexBuffer();
    renderer::IMaterial*     Material     = ResourceManager->createMaterial();

    VertexBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);
    VertexBuffer->setVertexFormat((renderer::EVF_VERTEX) | ((have_texcoords)?renderer::EVF_TEXCOORD:0) | ((have_normals)?renderer::EVF_NORMAL:0));
    VertexBuffer->setBufferType(renderer::EVBT_RAWDATA);

    SObjMtl DefaultMtl;
    DefaultMtl.VertexBuffer = VertexBuffer;
    DefaultMtl.Material     = Material;
    ObjMaterials["default"] = DefaultMtl;

    char string[256];
    while((file->read_string(string,256)) != 0)
    {

        switch(string[0])
        {

        case 'm': //mtllib
        {
            //extract mtl file name
            char mtl_file_name[256];
            sscanf(string,"mtllib %256s",mtl_file_name);
            //load .mtl file
            read_mtl_file(ObjMaterials,mtl_file_name);
            //read_mtl_file(&Materials,mtl_file_name);

            break;
        }
        case 'u': //usemtl
        {
            char mtl_name[256];
            sscanf(string,"usemtl %256s",mtl_name);

            std::map<std::string,SObjMtl>::iterator it;

            if((it = ObjMaterials.find(std::string(mtl_name))) != ObjMaterials.end())
            {
                VertexBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);
                VertexBuffer->setVertexFormat((renderer::EVF_VERTEX) | ((have_texcoords)?renderer::EVF_TEXCOORD:0) | ((have_normals)?renderer::EVF_NORMAL:0));
                VertexBuffer->setBufferType(renderer::EVBT_RAWDATA);

                VertexBuffer = (*it).second.VertexBuffer;
                Material     = (*it).second.Material;

            }
            else
            {

            }
            have_texcoords = false;
            have_normals   = false;
            break;
        }
        case 'v': //v/vt/vn
        {
            if(string[1] == ' ')
            {
                core::vector3f tmp_Vertex;
                sscanf(string + 2,"%f %f %f",&tmp_Vertex.x,&tmp_Vertex.y,&tmp_Vertex.z);
                Verticles.push_back(tmp_Vertex);
            }
            else if(string[1] == 't')
            {
                core::vector2f tmp_Texcoord;
                sscanf(string + 2,"%f %f",&tmp_Texcoord.x,&tmp_Texcoord.y);
                TexCoords.push_back(tmp_Texcoord);
            }
            else
            {
                core::vector3f tmp_Normal;
                sscanf(string + 2,"%f %f %f",&tmp_Normal.x,&tmp_Normal.y,&tmp_Normal.z);
                Normals.push_back(tmp_Normal);
            }
            break;
        }
        case 'f': //f
        {
            std::vector<core::vector3s> Face;

            char* tok = strtok(string + 1," \n");

            //identify index format
            int index_format = 0;
            for(u32 i = 0; i < strlen(tok); i++)
            {
                if(tok[i] == '/' && tok[i+1] == '/')
                {
                    index_format = 3;
                    break;
                }
                else if(tok[i] == '/')
                    index_format++;
            }

            while(tok != NULL)
            {
                core::vector3s tmp_Index;
                switch(index_format)
                {
                case 0:
                    sscanf(tok,"%d",&tmp_Index.x);
                    if(tmp_Index.x < 0)tmp_Index.x = Verticles.size() + tmp_Index.x;
                    else tmp_Index.x--;

                    break;
                case 1:
                    sscanf(tok,"%d/%d",&tmp_Index.x,&tmp_Index.y);

                    have_texcoords = true;

                    if(tmp_Index.x < 0)tmp_Index.x = Verticles.size() + tmp_Index.x;
                    else tmp_Index.x--;
                    if(tmp_Index.y < 0)tmp_Index.y = TexCoords.size() + tmp_Index.y;
                    else tmp_Index.y--;

                    break;
                case 2:
                    sscanf(tok,"%d/%d/%d",&tmp_Index.x,&tmp_Index.y,&tmp_Index.z);

                    have_texcoords = true;
                    have_normals   = true;

                    if(tmp_Index.x < 0)tmp_Index.x = Verticles.size() + tmp_Index.x;
                    else tmp_Index.x--;
                    if(tmp_Index.y < 0)tmp_Index.y = TexCoords.size() + tmp_Index.y;
                    else tmp_Index.y--;
                    if(tmp_Index.z < 0)tmp_Index.z = Normals.size()   + tmp_Index.z;
                    else tmp_Index.z--;

                    break;
                case 3:
                    sscanf(tok,"%d//%d",&tmp_Index.x,&tmp_Index.z);

                    have_normals   = true;

                    if(tmp_Index.x < 0)tmp_Index.x = Verticles.size() + tmp_Index.x;
                    else tmp_Index.x--;
                    if(tmp_Index.z < 0)tmp_Index.z = Normals.size()   + tmp_Index.z;
                    else tmp_Index.z--;

                    break;
                }
                Face.push_back(tmp_Index);
                tok = strtok(NULL," \n");
            }

            if(Face.size() > 3)
            {
                //!< If Face = Quad we use predefined vertex positions (it's little faster)
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
                    std::vector<core::vector3s> TriangulatedFaces;
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
            //Save to vertexbuffer
            u32 IndicesSize = Face.size();
            for(u32 i = 0; i < IndicesSize; i++)
            {
                renderer::SVertex Vertex;

                VertexBuffer->addPosition(Verticles[Face[i].x]);

                if(have_texcoords)
                    VertexBuffer->addUV(TexCoords[Face[i].y]);
                if(have_normals)
                    VertexBuffer->addNormal(Normals[Face[i].z]);
            }
            break;
        }

        }
    }
    /*


        Mesh->addMeshUnit(VertexBuffer,NULL);
        VertexBuffer->release();
    */
    for(auto it = ObjMaterials.begin(); it != ObjMaterials.end(); it++)
    {
        Mesh->addMeshUnit((*it).second.VertexBuffer,(*it).second.Material);
        (*it).second.VertexBuffer->release();
    }

    LOG_ENGINE_DEBUG("MESH LOADED\n");
    LOG_ENGINE_DEBUG("MESH_UNITS  : %d\n",Mesh->getMeshUnitsCount());
    LOG_ENGINE_DEBUG("MATERIALS   : %d\n",ObjMaterials.size());
    LOG_ENGINE_DEBUG("LOADED OBJECTS\n");

    return Mesh;
}

void CStaticMeshLoader_OBJ::read_mtl_file(std::map<std::string,SObjMtl>& materials,const char* mtl_file)
{
    io::IFile* mtl = FileSystem->open_file(mtl_file);

    renderer::IMaterial*     Material     = NULL;
    //parse mtl file
    char string[256];
    while((mtl->read_string(string,256)) != 0)
    {
        //---------
        switch(string[0])
        {
        case 'n':
        {
            char mtl_name[256];
            sscanf(string,"newmtl %256s",mtl_name);

            SObjMtl newMtl;
            newMtl.VertexBuffer = ResourceManager->createVertexBuffer();
            Material     = newMtl.Material     = ResourceManager->createMaterial();

            newMtl.VertexBuffer->setResourceName(mtl_name);

            LOG_ENGINE_DEBUG("new mtl loaded : %s\n",mtl_name);
            materials[std::string(mtl_name)] = newMtl;
        }
        break;
        case 'K':
            if(string[1] == 'd')
            {
                core::color4f color(0,0,0,1);
                sscanf(string + 2,"%f %f %f",&color.r,&color.g,&color.b);
                Material->setDiffuseColor(color);
            }
            break;
        case 'N':
            break;

        case 'm':
            char texture_name[256];
            sscanf(string,"map_Kd %256s",texture_name);
            renderer::ITexture* texture = ResourceManager->loadTexture(texture_name);
            Material->setTexture(texture,0);
            break;

        }
        //---------
    }
    mtl->release();
}

}
