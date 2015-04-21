#include "CAssimpMeshImporter.h"
#include "log/NLog.h"

#include "createVertexBuffer.h"
#include "createStaticMesh.h"
#include "createAnimatedMesh.h"

#include "IVertexBuffer.h"
#include "ERendererEnums.h"

namespace novaengine
{

CAssimpMeshImporter::CAssimpMeshImporter(io::IFileSystem* fs)
{
    FileSystem = fs;
    assimp_importer = new Assimp::Importer();
}

CAssimpMeshImporter::~CAssimpMeshImporter()
{
    delete assimp_importer;
}

bool CAssimpMeshImporter::isSupported(const char* file_extension)
{
    if(assimp_importer->IsExtensionSupported(file_extension))
        return 1;
    return 0;
}

bool CAssimpMeshImporter::isSupported(io::IFile* file)
{
    return false;
}

renderer::IStaticMesh* CAssimpMeshImporter::LoadStaticMesh(const char* path)
{
    core::string MeshPath = path;
    core::string ext = (MeshPath.find_last_of(".") != -1) ? MeshPath.substr(MeshPath.find_last_of(".")+1,MeshPath.length() - MeshPath.find_last_of(".")):"";


    io::IFile* mesh_file = FileSystem->open_file(path);

    if(!mesh_file)
        return NULL;

    renderer::IStaticMesh* mesh = LoadStaticMesh_assimp(mesh_file,ext.c_str());

    mesh_file->release();

    if(!mesh)
        return NULL;

    return mesh;
}

renderer::IStaticMesh* CAssimpMeshImporter::LoadStaticMesh(io::IFile* file,const char* hint)
{
    u32 FileStart = file->getPos();
    renderer::IStaticMesh* mesh = LoadAnimatedMesh_assimp(file,hint);
    file->seek(FileStart);
    return mesh;
}

renderer::IAnimatedMesh* CAssimpMeshImporter::LoadAnimatedMesh(const char* path)
{
    core::string MeshPath = path;
    core::string ext = (MeshPath.find_last_of(".") != -1) ? MeshPath.substr(MeshPath.find_last_of(".")+1,MeshPath.length() - MeshPath.find_last_of(".")):"";

    io::IFile* mesh_file = FileSystem->open_file(path);

    if(!mesh_file)
        return NULL;

    renderer::IAnimatedMesh* mesh = LoadAnimatedMesh_assimp(mesh_file,ext.c_str());

    mesh_file->release();

    if(!mesh)
        return NULL;

    return mesh;
}

renderer::IAnimatedMesh* CAssimpMeshImporter::LoadAnimatedMesh(io::IFile* file,const char* hint)
{
    u32 FileStart = file->getPos();
    renderer::IAnimatedMesh* mesh = LoadAnimatedMesh_assimp(file,hint);
    file->seek(FileStart);
    return mesh;
}

renderer::IStaticMesh* CAssimpMeshImporter::LoadStaticMesh_assimp(io::IFile* file,const char* Hint)
{
    char* FileData = new char[file->getSize() - file->getPos()];
    u32 rd  = file->read(FileData,file->getSize() - file->getPos());

    if(rd < file->getSize() - file->getPos())
        return NULL;

    const aiScene* assimp_scene = assimp_importer->ReadFileFromMemory(FileData,rd,aiProcess_PreTransformVertices |aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices,Hint);

    delete [] FileData;

    if(assimp_scene)
    {
        if(assimp_scene->HasMeshes())
        {
            renderer::IStaticMesh* mesh = renderer::createStaticMesh();

            for(u32 m = 0; m < assimp_scene->mNumMeshes; m++)
            {

                renderer::SVertex vert;
                renderer::IVertexBuffer* VertexBuffer = renderer::createVertexBuffer();
                std::vector<u32> Indices;

                for(u32 v = 0; v < assimp_scene->mMeshes[m]->mNumVertices; v++)
                {

                    vert.Position.x =  assimp_scene->mMeshes[m]->mVertices[v].x;
                    vert.Position.y =  assimp_scene->mMeshes[m]->mVertices[v].y;
                    vert.Position.z =  assimp_scene->mMeshes[m]->mVertices[v].z;

                    if(assimp_scene->mMeshes[m]->HasTextureCoords(0))
                    {
                        vert.TexCoord.x = assimp_scene->mMeshes[m]->mTextureCoords[0][v].x;
                        vert.TexCoord.y = assimp_scene->mMeshes[m]->mTextureCoords[0][v].y;
                    }

                    if(assimp_scene->mMeshes[m]->HasNormals())
                    {
                        vert.Normal.x =  assimp_scene->mMeshes[m]->mNormals[v].x;
                        vert.Normal.y =  assimp_scene->mMeshes[m]->mNormals[v].y;
                        vert.Normal.z =  assimp_scene->mMeshes[m]->mNormals[v].z;
                    }

                    if(assimp_scene->mMeshes[m]->HasVertexColors(0))
                    {
                        vert.VertexColor.Red    = assimp_scene->mMeshes[m]->mColors[0][v].r;
                        vert.VertexColor.Green  = assimp_scene->mMeshes[m]->mColors[0][v].g;
                        vert.VertexColor.Blue   = assimp_scene->mMeshes[m]->mColors[0][v].b;
                        vert.VertexColor.Alpha  = assimp_scene->mMeshes[m]->mColors[0][v].a;
                    }

                    VertexBuffer->addVertex(vert);
                }

                for(u32 f = 0; f < assimp_scene->mMeshes[m]->mNumFaces; f++)
                    for(u32 i = 0; i < assimp_scene->mMeshes[m]->mFaces[f].mNumIndices; i++)
                        Indices.push_back(assimp_scene->mMeshes[m]->mFaces[f].mIndices[i]);


                VertexBuffer->setPrimitiveType(renderer::EPT_TRIANGLES);
                VertexBuffer->setBufferType(renderer::EVBT_RAWDATA);
                VertexBuffer->setIndices(Indices.data(),Indices.size());
                //------------------------------------------------
                u32 VertexFormat = renderer::EVF_VERTEX;
                if(assimp_scene->mMeshes[m]->HasTextureCoords(0));
                VertexFormat |= renderer::EVF_TEXCOORD;
                if(assimp_scene->mMeshes[m]->HasNormals());
                VertexFormat |= renderer::EVF_NORMAL;
                VertexBuffer->setVertexFormat(VertexFormat);
                //------------------------------------------------

                mesh->addMeshUnit(VertexBuffer,NULL);

                VertexBuffer->release();
            }

            assimp_importer->FreeScene();
            return mesh;
        }
    }
    else
    {
        io::log::debug("oops... assimp cant load mesh...\n");
        return NULL;
    }
}

renderer::IAnimatedMesh* CAssimpMeshImporter::LoadAnimatedMesh_assimp(io::IFile* file,const char* Hint)
{
    char* FileData = new char[file->getSize() - file->getPos()];
    u32 rd  = file->read(FileData,file->getSize() - file->getPos());

    if(rd < file->getSize() - file->getPos())
        return NULL;

    const aiScene* assimp_scene = assimp_importer->ReadFileFromMemory(FileData,rd, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices,Hint);

    delete [] FileData;

    if(assimp_scene)
    {
        if(assimp_scene->HasMeshes())
        {
            renderer::IAnimatedMesh* mesh = renderer::createAnimatedMesh();

            for(u32 m = 0; m < assimp_scene->mNumMeshes; m++)
            {
                renderer::IVertexBuffer*        VertexBuffer = renderer::createVertexBuffer();
                renderer::IVertexWeightBuffer*  WeightBuffer = new renderer::IVertexWeightBuffer();
                renderer::SJoints               Joints;
                std::vector<u32> Indices;
                //----------------------------------------------------------------------------------------------
                LoadJointsData(assimp_scene->mRootNode,assimp_scene->mMeshes[m],&Joints);
                mesh->setJoints(Joints);
                //----------------------------------------------------------------------------------------------
                LoadVertexWeightsData(assimp_scene->mMeshes[m],&Joints,WeightBuffer);
                //----------------------------------------------------------------------------------------------
                renderer::SVertex           vert;

                for(u32 v = 0; v < assimp_scene->mMeshes[m]->mNumVertices; v++)
                {

                    vert.Position.x =  assimp_scene->mMeshes[m]->mVertices[v].x;
                    vert.Position.y =  assimp_scene->mMeshes[m]->mVertices[v].y;
                    vert.Position.z =  assimp_scene->mMeshes[m]->mVertices[v].z;

                    if(assimp_scene->mMeshes[m]->HasTextureCoords(0))
                    {
                        vert.TexCoord.x = assimp_scene->mMeshes[m]->mTextureCoords[0][v].x;
                        vert.TexCoord.y = assimp_scene->mMeshes[m]->mTextureCoords[0][v].y;
                    }

                    if(assimp_scene->mMeshes[m]->HasNormals())
                    {
                        vert.Normal.x =  assimp_scene->mMeshes[m]->mNormals[v].x;
                        vert.Normal.y =  assimp_scene->mMeshes[m]->mNormals[v].y;
                        vert.Normal.z =  assimp_scene->mMeshes[m]->mNormals[v].z;
                    }

                    if(assimp_scene->mMeshes[m]->HasVertexColors(0))
                    {
                        vert.VertexColor.Red    = assimp_scene->mMeshes[m]->mColors[0][v].r;
                        vert.VertexColor.Green  = assimp_scene->mMeshes[m]->mColors[0][v].g;
                        vert.VertexColor.Blue   = assimp_scene->mMeshes[m]->mColors[0][v].b;
                        vert.VertexColor.Alpha  = assimp_scene->mMeshes[m]->mColors[0][v].a;
                    }

                    VertexBuffer->addVertex(vert);
                }

                for(u32 f = 0; f < assimp_scene->mMeshes[m]->mNumFaces; f++)
                    for(u32 i = 0; i < assimp_scene->mMeshes[m]->mFaces[f].mNumIndices; i++)
                        Indices.push_back(assimp_scene->mMeshes[m]->mFaces[f].mIndices[i]);


                VertexBuffer->setPrimitiveType(renderer::EPT_TRIANGLES);
                VertexBuffer->setBufferType(renderer::EVBT_RAWDATA);
                VertexBuffer->setIndices(Indices.data(),Indices.size());
                //------------------------------------------------
                u32 VertexFormat = renderer::EVF_VERTEX;
                if(assimp_scene->mMeshes[m]->HasTextureCoords(0));
                VertexFormat |= renderer::EVF_TEXCOORD;
                if(assimp_scene->mMeshes[m]->HasNormals());
                VertexFormat |= renderer::EVF_NORMAL;
                VertexBuffer->setVertexFormat(VertexFormat);
                //----------------------------------------------------------------------------------
                mesh->addAnimatedMeshUnit(VertexBuffer,WeightBuffer,NULL);
                //----------------------------------------------------------------------------------


                for(u32 a = 0; a < assimp_scene->mNumAnimations; a++)
                {
                    renderer::SAnimationFrame Frame;
                    renderer::SJointTransform JointTransform;
                    bool AnimationLoaded = false;

                    io::log::debug("AnimationNum : %d ChannelsCount : %d\n",a,assimp_scene->mAnimations[a]->mNumChannels);

                    for(u32 c = 0; c < assimp_scene->mAnimations[a]->mNumChannels; c++)
                    {

                        io::log::debug(" - ChannelName : %s\n",assimp_scene->mAnimations[a]->mChannels[c]->mNodeName.C_Str());

                        for(u32 j = 0; j < Joints.JointsArray.size(); j++)
                        {
                            if(strcmp(assimp_scene->mAnimations[a]->mChannels[c]->mNodeName.C_Str(),Joints.JointsArray[j].Name.c_str()) == 0)
                            {
                                u32 KeyFrameCount = assimp_scene->mAnimations[a]->mChannels[c]->mNumPositionKeys;
                                u32 CurrentKeyFrame = 0;

                                if(KeyFrameCount == 0)
                                    break;

                                for(u32 f = 0; f < KeyFrameCount; f++)
                                {
                                    io::log::debug("FRAME #%d\n",CurrentKeyFrame);
                                    for(u32 ic = 0; ic < assimp_scene->mAnimations[a]->mNumChannels; ic++)
                                    {
                                        for(u32 ij = 0; ij < Joints.JointsArray.size(); ij++)
                                        {
                                            if(strcmp(assimp_scene->mAnimations[a]->mChannels[ic]->mNodeName.C_Str(),Joints.JointsArray[ij].Name.c_str()) == 0)
                                            {
                                                io::log::debug(" loaded keyframe for %d(%s)\n",ij,Joints.JointsArray[ij].Name.c_str());
                                                //-----------------------------------------------------------------------------------------------------------
                                                JointTransform.JointIndex = ij;

                                                JointTransform.Position.x   = assimp_scene->mAnimations[a]->mChannels[ic]->mPositionKeys[CurrentKeyFrame].mValue.x;
                                                JointTransform.Position.y   = assimp_scene->mAnimations[a]->mChannels[ic]->mPositionKeys[CurrentKeyFrame].mValue.y;
                                                JointTransform.Position.z   = assimp_scene->mAnimations[a]->mChannels[ic]->mPositionKeys[CurrentKeyFrame].mValue.z;

                                                aiMatrix3x3   aiRot = assimp_scene->mAnimations[a]->mChannels[ic]->mRotationKeys[CurrentKeyFrame].mValue.GetMatrix();

                                                core::matrixf RotMatrix;

                                                RotMatrix[0]  = aiRot.a1;
                                                RotMatrix[4]  = aiRot.b1;
                                                RotMatrix[8]  = aiRot.c1;

                                                RotMatrix[1]  = aiRot.a2;
                                                RotMatrix[5]  = aiRot.b2;
                                                RotMatrix[9]  = aiRot.c2;

                                                RotMatrix[2]  = aiRot.a3;
                                                RotMatrix[6]  = aiRot.b3;
                                                RotMatrix[10] = aiRot.c3;

                                                RotMatrix[15] = 1;

                                                core::vector3f Rotation = RotMatrix.getRotationDegrees();

                                                JointTransform.Rotation.x = Rotation.x;
                                                JointTransform.Rotation.y = Rotation.y;
                                                JointTransform.Rotation.z = Rotation.z;

                                                //assimp_scene->mAnimations[a]->mChannels[c]->mRotationKeys[n].mValue.GetMatrix().Rotation()

                                                Frame.JointsTransformations.push_back(JointTransform);

                                                io::log::debug("  - p %f %f %f\n",JointTransform.Position.x,JointTransform.Position.y,JointTransform.Position.z);
                                                io::log::debug("  - r %f %f %f\n",JointTransform.Rotation.x,JointTransform.Rotation.y,JointTransform.Rotation.z);

                                                //----------------------------------------------------------------------------------------------------------
                                            }
                                        }
                                    }
                                    CurrentKeyFrame++;
                                    mesh->addAnimationFrame(Frame);
                                    Frame.JointsTransformations.clear();
                                }
                                AnimationLoaded = true;
                            }
                        }
                        if(AnimationLoaded)
                            break;
                    }
                }

                VertexBuffer->release();
                WeightBuffer->release();
            }

            assimp_importer->FreeScene();
            return mesh;
        }
    }
    else
    {
        io::log::debug("oops... assimp cant load mesh...\n");
        return NULL;
    }
}

int CAssimpMeshImporter::LoadJointsData(aiNode* RootNode,aiMesh* Mesh,renderer::SJoints* Joints)
{

    for(u32 b = 0; b < Mesh->mNumBones; b++)
        LoadJointRecursively(Mesh->mBones[b],RootNode,Mesh,Joints);
}

int CAssimpMeshImporter::LoadJointRecursively(aiBone* Bone,aiNode* RootNode,aiMesh* Mesh,renderer::SJoints* Joints)
{
    s32 ParentIndex = -1;
    //----------------------------
    aiNode* CurrentNode = RootNode->FindNode(Bone->mName);
    aiNode* ParentNode  = CurrentNode->mParent;

    if(not ParentNode)
        ParentIndex = -1;
    //----------------------------


    s32 BoneID = -1;
    for(u32 bone_num = 0; bone_num < Mesh->mNumBones; bone_num++)
    {
        if(strcmp(ParentNode->mName.C_Str(),Mesh->mBones[bone_num]->mName.C_Str()) == 0)
        {
            BoneID = bone_num;
            break;
        }
    }

    if(BoneID == -1)
    {
        ParentIndex = -1;
    }
    else
    {
        s32 JointIndex = -1;
        for(u32 joint_num = 0; joint_num < Joints->JointsArray.size(); joint_num++)
        {
            if(strcmp(Joints->JointsArray[joint_num].Name.c_str(),ParentNode->mName.C_Str()) == 0)
            {
                JointIndex = joint_num;
                break;
            }
        }

        if(JointIndex == -1)
            ParentIndex = LoadJointRecursively(Mesh->mBones[BoneID],RootNode,Mesh,Joints);
        else
            ParentIndex = JointIndex;


    }

    //----------------------------

    renderer::SJoint            joint;
    renderer::SVertexWeight     svert;
    renderer::SWeight           wvert;

    aiVector3D      JointScale;
    aiVector3D      JointPosition;
    aiVector3D      JointRotation;
    aiQuaternion    qRotation;

    Bone->mOffsetMatrix.DecomposeNoScaling(qRotation,JointPosition);
    core::quaternionf qut(qRotation.x,qRotation.y,qRotation.z,qRotation.w);

    if(ParentIndex == -1 && ParentNode!=0)
    {

    }

    joint.Name = Bone->mName.C_Str();
    joint.Position.set(JointPosition.x,JointPosition.y,JointPosition.z);
    joint.Rotation.set(0,0,0);
    joint.ParentIndex = ParentIndex;

    Joints->JointsArray.push_back(joint);

    //io::log::debug("BONE (%s)\n\tPOSITION %f %f %f\n\tROTATION %f %f %f\n",Bone->mName.C_Str(),JointPosition.x,JointPosition.y,JointPosition.z,joint.Rotation.x,joint.Rotation.y,joint.Rotation.z);

    return Joints->JointsArray.size()-1;
}
int CAssimpMeshImporter::LoadVertexWeightsData(aiMesh* Mesh,renderer::SJoints* Joints,renderer::IVertexWeightBuffer* Weights)
{
    for(u32 b = 0; b < Mesh->mNumBones; b++)
    {
        aiBone* Bone = Mesh->mBones[b];
        renderer::SWeight       weight;
        renderer::SVertexWeight vertex_weight;

        s32 JointIndex = -1;
        for(u32 j = 0; j < Joints->JointsArray.size(); j++)
        {
            if(strcmp(Bone->mName.C_Str(),Joints->JointsArray[j].Name.c_str()) == 0)
            {
                JointIndex = j;
                break;
            }
        }

        for(u32 w = 0; w < Bone->mNumWeights; w++)
        {

            aiVector3D VertexPosition = Mesh->mVertices[Bone->mWeights[w].mVertexId];

            weight.Position.set(VertexPosition.x,VertexPosition.y,VertexPosition.z);
            //weight.Position    = Bone->mWeights[w].
            weight.Weight      = Bone->mWeights[w].mWeight;
            weight.JointIndex  = JointIndex;

            s32 WeightBufferIndex = -1;

            for(u32 we = 0; we < Weights->WeightBuffer.size(); we++)
            {
                if(Weights->WeightBuffer[we].VertexIndex == Bone->mWeights[w].mVertexId)
                {
                    WeightBufferIndex = we;
                    break;
                }
            }


            if(WeightBufferIndex != -1)
            {
                //io::log::debug("#%d(a) WEIGHT %f OF %d POSITION %f %f %f AROUND %d\n",w,weight.Weight,Weights->WeightBuffer[WeightBufferIndex].VertexIndex,VertexPosition.x,VertexPosition.y,VertexPosition.z,weight.JointIndex);
                Weights->WeightBuffer[WeightBufferIndex].Weights.push_back(weight);
            }
            else
            {
                vertex_weight.VertexIndex = Bone->mWeights[w].mVertexId;
                vertex_weight.Weights.push_back(weight);
                Weights->WeightBuffer.push_back(vertex_weight);
                //io::log::debug("#%d(n) WEIGHT %f OF %d POSITION %f %f %f AROUND %d\n",w,weight.Weight,vertex_weight.VertexIndex,VertexPosition.x,VertexPosition.y,VertexPosition.z,weight.JointIndex);
                vertex_weight.Weights.clear();
            }


        }
    }
    return 1;
}
}
