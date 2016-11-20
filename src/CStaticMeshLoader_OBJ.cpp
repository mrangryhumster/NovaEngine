#include "CStaticMeshLoader_OBJ.h"

#ifdef _NE_INCLUDE_MESH_LOADER_OBJ

#include "CStaticMesh.h"
#include "CMeshBuffer.h"

namespace novaengine
{

CStaticMeshLoader_OBJ::CStaticMeshLoader_OBJ(io::IFileSystem* FileSystem,IResourceManager* ResourceManager) :
	m_FileSystem(FileSystem),
	m_ResourceManager(ResourceManager)
{
}
//-----------------------------------------------------------------------------------------------
CStaticMeshLoader_OBJ::~CStaticMeshLoader_OBJ()
{

}
//-----------------------------------------------------------------------------------------------
bool CStaticMeshLoader_OBJ::isSupported(const char* file_extension)
{
    if(strcmp(file_extension,"obj") == 0)
        return 1;
    return 0;
}
//-----------------------------------------------------------------------------------------------
bool CStaticMeshLoader_OBJ::isSupported(io::IFile* file)
{
    //! I'm too lazy to write this...
    return false;
}
//-----------------------------------------------------------------------------------------------
renderer::IStaticMesh* CStaticMeshLoader_OBJ::loadStaticMesh(const char* path)
{
    io::IFile* mesh_file = m_FileSystem->open_file(path);

    if(!mesh_file)
        return nullptr;

    renderer::IStaticMesh* mesh = LoadOBJ(mesh_file);

    mesh_file->release();

    if(!mesh)
        return nullptr;

    return mesh;
}
//-----------------------------------------------------------------------------------------------
renderer::IStaticMesh* CStaticMeshLoader_OBJ::loadStaticMesh(io::IFile* file,const char* hint)
{
    u32 FileStart = file->getPos();
    renderer::IStaticMesh* mesh = LoadOBJ(file);
    file->seek(FileStart);
    return mesh;
}
//-----------------------------------------------------------------------------------------------
renderer::IStaticMesh* CStaticMeshLoader_OBJ::LoadOBJ(io::IFile* file)
{
    //CHECK_CODE

    bool have_texcoords = false;
    bool have_normals   = false;

    std::vector<core::vector3f> Verticles;
    std::vector<core::vector2f> TexCoords;
    std::vector<core::vector3f> Normals;

    //! All meshes in this loader grouping by material
    std::map<std::string,renderer::IMeshBuffer*> OBJ_MaterialGroups;

    renderer::IStaticMesh* Mesh = m_ResourceManager->createStaticMesh();

    renderer::IMeshBuffer* MeshBuffer = nullptr;
    //-------------------------------
    //! default object/material
    OBJ_MaterialGroups["default"] = MeshBuffer = m_ResourceManager->createMeshBuffer();
    MeshBuffer->createBuffer(0,renderer::EVA_POSITION);
	MeshBuffer->createBuffer(1, renderer::EVA_TEXCOORD);
	MeshBuffer->createBuffer(2, renderer::EVA_NORMAL);

    MeshBuffer->lock();

    //-------------------------------

    char string[256];
    while((file->read_string(string,256)) != 0)
    {

        switch(string[0])
        {
			/*
        case 'm': //mtllib
        {
			//If file opened from hdd then we try to connect mtl file
            if(file->getSourceType() == io::EFST_NATIVE)
            {
                //extract mtl file name
                char mtl_file_name[256];
                sscanf(string,"mtllib %255s",mtl_file_name);

				std::string full_mtl_path = "";
				full_mtl_path += file->getDir();
				full_mtl_path += mtl_file_name;

                //load .mtl file
				read_mtl_file(OBJ_MaterialGroups,full_mtl_path.c_str());
            }
            break;
        }
        case 'u': //usemtl
        {
            char mtl_name[256];
            sscanf(string,"usemtl %255s",mtl_name);

            std::map<std::string,renderer::IMeshBuffer*>::iterator it;
            if((it = OBJ_MaterialGroups.find(std::string(mtl_name))) != OBJ_MaterialGroups.end())
                MeshBuffer = (*it).second;
            else
                MeshBuffer = OBJ_MaterialGroups["default"];


            have_texcoords = false;
            have_normals   = false;
            break;
        }
		*/
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

            //!identify index format
            int index_format = 0;
			size_t strl = strlen(tok);
            for(u32 i = 0; i < strl; i++)
            {
                if(tok[i] == '/' && tok[i+1] == '/')
                {
                    index_format = 3;
                    break;
                }
                else if(tok[i] == '/')
                    index_format++;
            }
            //! Possible 4 formats
            //! 1 : POSITION
            //! 2 : POSITION/TEXCOORD
            //! 3 : POSITION/TEXCOORD/NORMAL
            //! 4 : POSITION//NORMAL  (no texcoord)
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

            //! Triangulate faces
            if(Face.size() > 3)
            {
                //! If Face = Quad we use predefined vertex positions (it's little faster)
                //! Else we split polygon(more than 4 vertex) on triangles
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

            //!Save to MeshBuffer
            u32 IndicesSize = Face.size();
            for(u32 i = 0; i < IndicesSize; i++)
            {

                MeshBuffer->addBufferData(0,&Verticles[Face[i].x],sizeof(core::vector3f));

                if(have_texcoords)
                    MeshBuffer->addBufferData(1,&TexCoords[Face[i].y],sizeof(core::vector2f));

                if(have_normals)
                    MeshBuffer->addBufferData(2,&Normals[Face[i].z],sizeof(core::vector3f));

            }
            break;
        }

        }
    }

    for(auto it = OBJ_MaterialGroups.begin(); it != OBJ_MaterialGroups.end(); it++)
    {
        renderer::IMeshBuffer* MeshBuffer = (*it).second;
        MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);
        MeshBuffer->unlock();
        Mesh->addMeshBuffer(MeshBuffer);
        /*
        if(MeshBuffer->getBufferSize(renderer::EVA_POSITION) > 0)
        {
            MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);

            Mesh->addMeshBuffer(MeshBuffer);
        }
        */
        //MeshBuffer->release();
    }

    LOG_ENGINE_DEBUG("MESH LOADED\n");
    LOG_ENGINE_DEBUG("MESHBUFFERS COUNT  : %d\n",Mesh->getMeshBuffersCount());

    return Mesh;
}
//-----------------------------------------------------------------------------------------------
void CStaticMeshLoader_OBJ::read_mtl_file(std::map<std::string,renderer::IMeshBuffer*>& obj_materialgroups,const char* mtl_file)
{

    io::IFile* mtl = m_FileSystem->open_file(mtl_file);
    if(mtl == NULL)
        return;
    renderer::IMaterial* Material = nullptr;

    //parse mtl file
    char string[256];
    while((mtl->read_string(string,256)) != 0)
    {
        //---------
        u32 i = 0;
        while(string[i] == ' ' || string[i] == '\t')
            i++;
        switch(string[i])
        {
        case 'n': //newmtl
        {
            char mtl_name[256];
            sscanf(&string[i],"newmtl %255s",mtl_name);

            if(strcmp(mtl_name,"default") == 0)
            {
                LOG_ENGINE_DEBUG("warning: default mtl redefined\n");

                Material = m_ResourceManager->createMaterial();
                obj_materialgroups["default"]->setMaterial(Material);
                Material->release();
            }
            else
            {
                LOG_ENGINE_DEBUG("new mtl loaded : %s\n",mtl_name);

                Material = m_ResourceManager->createMaterial();
                Material->setObjectName(mtl_name);
                renderer::IMeshBuffer* MeshBuffer = m_ResourceManager->createMeshBuffer();
                MeshBuffer->setMaterial(Material);
                Material->release();

                obj_materialgroups[std::string(mtl_name)] = MeshBuffer;
            }
        }
        break;
        case 'K': //Ka/Kd/Ks
            if(string[i+1] == 'd')
            {
                core::color4f color(0,0,0,1);
                sscanf(&string[i+2],"%f %f %f",&color.r,&color.g,&color.b);
//                Material->setDiffuseColor(color);
            }
            break;
        case 'N': //Ns/Ni
            break;

        case 'm': //map_Kd
            char texture_name[256];
            sscanf(&string[i],"map_Kd %255s",texture_name);

			std::string full_texture_path = "";
			full_texture_path += mtl->getDir();
			full_texture_path += texture_name;

			renderer::ITexture* Texture = m_ResourceManager->loadTexture(full_texture_path.c_str());
            if(Texture)
            {
//                Material->setTexture(Texture,0);
                Texture->release();
            }
            break;

        }
        //---------
    }
    mtl->release();
}

}

#endif // _NE_INCLUDE_MESH_LOADER_OBJ
