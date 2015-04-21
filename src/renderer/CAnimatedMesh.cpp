#include "CAnimatedMesh.h"
//#include "log/NLog.h"
#include <assert.h>

namespace novaengine
{
namespace renderer
{

CAnimatedMesh::CAnimatedMesh():
    AnimatedMeshUnitsCount(0)
{
    //ctor
}

CAnimatedMesh::~CAnimatedMesh()
{
    if(AnimatedMeshUnits.size() > 0)
    {
        u32 UnitsCount = AnimatedMeshUnits.size();
        for(u32 i = 0; i < UnitsCount; i++)
        {
            AnimatedMeshUnits[i].VertexBuffer->release();
            AnimatedMeshUnits[i].WeightBuffer->release();

            if(AnimatedMeshUnits[i].Material)
                AnimatedMeshUnits[i].Material->release();

        }
        AnimatedMeshUnits.clear();
    }
}


u32  CAnimatedMesh::getMeshUnitsCount()
{
    return AnimatedMeshUnitsCount;
}

void CAnimatedMesh::addAnimatedMeshUnit(SAnimatedMeshUnit MeshUnit)
{
    if(MeshUnit.VertexBuffer == NULL)
        return;

    MeshUnit.VertexBuffer->capture();
    MeshUnit.WeightBuffer->capture();

    if(MeshUnit.Material)
        MeshUnit.Material->capture();

    AnimatedMeshUnits.push_back(MeshUnit);

    AnimatedMeshUnitsCount = AnimatedMeshUnits.size();
}

void CAnimatedMesh::addAnimatedMeshUnit(IVertexBuffer* VertexBuffer ,IVertexWeightBuffer* WeightBuffer,IMaterial* Material)
{
    if(!VertexBuffer)
        return;

    VertexBuffer->capture();
    WeightBuffer->capture();

    if(Material)
        Material->capture();

    SAnimatedMeshUnit NewMeshUnit;
    NewMeshUnit.VertexBuffer = VertexBuffer;
    NewMeshUnit.WeightBuffer = WeightBuffer;
    NewMeshUnit.Material     = Material;

    AnimatedMeshUnits.push_back(NewMeshUnit);

    for(u32 i = 0; i < WeightBuffer->WeightBuffer.size(); i++)
    {
        f32 accum = 0;
        for(u32 w = 0; w < WeightBuffer->WeightBuffer[i].Weights.size(); w++)
        {
            accum += WeightBuffer->WeightBuffer[i].Weights[w].Weight;
        }
        //LOG_ENGINE_DEBUG" VERTEX %d TOTAL WEIGTH %f IN %d JOINTS", WeightBuffer->WeightBuffer[i].VertexIndex,accum,WeightBuffer->WeightBuffer[i].Weights.size());
    }

    AnimatedMeshUnitsCount = AnimatedMeshUnits.size();
}

const SMeshUnit* CAnimatedMesh::getMeshUnit(u32 id)
{
    if(id+1 > AnimatedMeshUnitsCount)
        return NULL;

    SMeshUnit MeshUnit;
    MeshUnit.VertexBuffer = AnimatedMeshUnits[id].VertexBuffer;
    MeshUnit.Material     = AnimatedMeshUnits[id].Material;

    return &MeshUnit;
}

SAnimatedMeshUnit CAnimatedMesh::getAnimatedMeshUnit(u32 id)
{
    if(id+1 > AnimatedMeshUnitsCount)
        return SAnimatedMeshUnit();
    return AnimatedMeshUnits[id];
}


void CAnimatedMesh::removeMeshUnit(u32 id)
{
    if(id+1 > AnimatedMeshUnitsCount)
        return ;

    SAnimatedMeshUnit MeshUnit = AnimatedMeshUnits[id];
    MeshUnit.VertexBuffer->release();
    MeshUnit.WeightBuffer->release();
    if(MeshUnit.Material)
        MeshUnit.Material->release();
    AnimatedMeshUnits.erase(AnimatedMeshUnits.begin()+id);

    AnimatedMeshUnitsCount = AnimatedMeshUnits.size();
}

IVertexBuffer* CAnimatedMesh::getVertexBuffer(u32 id)
{
    if(id+1 > AnimatedMeshUnitsCount)
        return NULL;
    return AnimatedMeshUnits[id].VertexBuffer;
}

IMaterial* CAnimatedMesh::getMaterial(u32 id)
{
    if((id+1) > AnimatedMeshUnitsCount)
        return NULL;
    return AnimatedMeshUnits[id].Material;
}

//! Lets rock
void CAnimatedMesh::setJoints(SJoints newJoints)
{
    Joints.JointsArray = newJoints.JointsArray;
}

SJoints* CAnimatedMesh::getJoints()
{
    return &Joints;
}

void CAnimatedMesh::addAnimation(SAnimation newAnimation)
{
    Animation = newAnimation;
}
void CAnimatedMesh::applyAnimationByTime(f32 Time)
{
    if(Time >= Animation.Lenght)
        Time = Animation.Lenght;

    for(u32 frame = 0; frame < Animation.KeyFrames.size(); frame++)
    {
        if(math::is_equalf(Animation.KeyFrames[frame].TimePosition,Time) || ((Animation.KeyFrames[frame].TimePosition > Time) && frame == 0))
        {
            SAnimationKeyFrame& KeyFrame = Animation.KeyFrames[frame];

            for(u32 i = 0; i < KeyFrame.Transformations.size(); i++)
            {
                SJointTransformation& Transform = KeyFrame.Transformations[i];
                Joints.JointsArray[Transform.JointIndex].Position = Transform.Position;
                Joints.JointsArray[Transform.JointIndex].Rotation = Transform.Rotation;
                Joints.JointsArray[Transform.JointIndex].TransformUpdated = false;
            }

            break;
        }
        else if(Animation.KeyFrames[frame].TimePosition > Time)
        {
            SAnimationKeyFrame& FirstKeyFrame  = Animation.KeyFrames[frame-1];
            SAnimationKeyFrame& SecondKeyFrame = Animation.KeyFrames[frame];
            f32 Interpolation = (Time - FirstKeyFrame.TimePosition) / (SecondKeyFrame.TimePosition - FirstKeyFrame.TimePosition);

            for(u32 i = 0; i < FirstKeyFrame.Transformations.size(); i++)
            {
                SJointTransformation& FirstTransform  = FirstKeyFrame.Transformations[i];
                SJointTransformation& SecondTransform = SecondKeyFrame.Transformations[i];

                Joints.JointsArray[FirstTransform.JointIndex].Position = FirstTransform.Position +  (SecondTransform.Position - FirstTransform.Position) * Interpolation;
                Joints.JointsArray[FirstTransform.JointIndex].Rotation.slerp_this(FirstTransform.Rotation,SecondTransform.Rotation,Interpolation);
                Joints.JointsArray[FirstTransform.JointIndex].TransformUpdated = false;
            }

            break;
        }
    }


    u32 JointCount = Joints.JointsArray.size();
    for(u32 j = 0; j < JointCount; j++)
        update_joint_tranform(&Joints.JointsArray[j]);

    update_vertex_transform();
}

void CAnimatedMesh::applyAnimationByFrame(u32 Frame)
{
    SAnimationKeyFrame& KeyFrame = Animation.KeyFrames[Frame];
    for(u32 i = 0; i < KeyFrame.Transformations.size(); i++)
    {
        Joints.JointsArray[KeyFrame.Transformations[i].JointIndex].Position = KeyFrame.Transformations[i].Position;
        Joints.JointsArray[KeyFrame.Transformations[i].JointIndex].Rotation = KeyFrame.Transformations[i].Rotation;
        Joints.JointsArray[KeyFrame.Transformations[i].JointIndex].TransformUpdated = false;
    }
    u32 JointCount = Joints.JointsArray.size();
    for(u32 j = 0; j < JointCount; j++)
        update_joint_tranform(&Joints.JointsArray[j]);

    update_vertex_transform();
}
void CAnimatedMesh::applyAnimationByTime(f32 StartTime,f32 EndTime,f32 DeltaTime)
{
    if(DeltaTime > 1.0f)
        DeltaTime = 1.0f;
    else if(DeltaTime < 0.0f)
        DeltaTime = 0.0f;

    applyAnimationByTime(StartTime + (EndTime - StartTime) * DeltaTime);
}
void CAnimatedMesh::applyAnimationByFrame(u32 StartFrame,u32 EndFrame,f32 DeltaTime)
{
    if(DeltaTime > 1.0f)
        DeltaTime = 1.0f;
    else if(DeltaTime < 0.0f)
        DeltaTime = 0.0f;

    u32  FirstFrameID = 0;
    u32 SecondFrameID = 0;
    f64 Interpolation = 0;

    f64  FirstFrameTime = Animation.KeyFrames[StartFrame].TimePosition;
    f64 SecondFrameTime = Animation.KeyFrames[EndFrame].TimePosition;
    f64 TargetFrameTime = FirstFrameTime + (SecondFrameTime - FirstFrameTime) * DeltaTime;

    u32 KeyFramesCount = Animation.KeyFrames.size();
    if(KeyFramesCount > 1)
    {
        for(u32 i = 0; i < KeyFramesCount; i++)
        {
            if(Animation.KeyFrames[i].TimePosition > TargetFrameTime)
            {
                if(i > 0)
                {
                    FirstFrameID  = i-1;
                    SecondFrameID = i;
                    break;
                }
                else
                {
                    applyAnimationByFrame(0);
                    break;
                }
            }
        }
    }
    else
    {
        FirstFrameID  = 0;
        SecondFrameID = 0;
    }
    //LOG_ENGINE_DEBUG" frames %d:%d",FirstFrameID,SecondFrameID);
    if(FirstFrameID != SecondFrameID)
    {
        SAnimationKeyFrame&  FirstKeyFrame = Animation.KeyFrames[FirstFrameID];
        SAnimationKeyFrame& SecondKeyFrame = Animation.KeyFrames[SecondFrameID];
        Interpolation = (TargetFrameTime - FirstKeyFrame.TimePosition) / (SecondKeyFrame.TimePosition - FirstKeyFrame.TimePosition);

        u32 FirstKeyFrameTransformationsCount  = FirstKeyFrame.Transformations.size();
        u32 SecondKeyFrameTransformationsCount = SecondKeyFrame.Transformations.size();

        SJointTransformation *FirstTransformation  = 0;
        SJointTransformation *SecondTransformation = 0;

        for(u32 i_1 = 0; i_1 < FirstKeyFrameTransformationsCount; i_1++)
        {
            FirstTransformation  = &FirstKeyFrame.Transformations[i_1];

            if((FirstFrameID == 328 || FirstFrameID == 329 ||FirstFrameID == 330) && FirstTransformation->JointIndex == 1)
            {
                //LOG_ENGINE_DEBUG"Frame:%d Joint:%d rot:(%f,%f,%f,%f)",FirstFrameID,FirstTransformation->JointIndex,FirstTransformation->Rotation.x,FirstTransformation->Rotation.y,FirstTransformation->Rotation.z,FirstTransformation->Rotation.w);
            }
            for(u32 i_2 = 0; i_2 < SecondKeyFrameTransformationsCount; i_2++)
            {
                if(FirstTransformation->JointIndex == SecondKeyFrame.Transformations[i_2].JointIndex)
                {
                    SecondTransformation = &SecondKeyFrame.Transformations[i_2];
                    Joints.JointsArray[FirstTransformation->JointIndex].Position = FirstTransformation->Position + (SecondTransformation->Position - FirstTransformation->Position) * Interpolation;
                    Joints.JointsArray[FirstTransformation->JointIndex].Rotation.slerp_this(FirstTransformation->Rotation,SecondTransformation->Rotation,Interpolation);
                    Joints.JointsArray[FirstTransformation->JointIndex].TransformUpdated = false;
                    break;
                }
            }
        }
    }
    else
    {

    }
    u32 JointCount = Joints.JointsArray.size();
    for(u32 j = 0; j < JointCount; j++)
        update_joint_tranform(&Joints.JointsArray[j]);

    update_vertex_transform();
}
IStaticMesh* CAnimatedMesh::getCopy()
{
    return NULL;
}
void CAnimatedMesh::update_joint_tranform(SJoint* joint)
{
    if(joint->TransformUpdated)
        return;

    joint->TransformUpdated = true;

    if(joint->ParentIndex > -1)
    {
        update_joint_tranform(&Joints.JointsArray[joint->ParentIndex]);
        joint->Rotation  = Joints.JointsArray[joint->ParentIndex].Rotation * joint->Rotation;
        joint->Position  = Joints.JointsArray[joint->ParentIndex].Rotation.rotate_vector(joint->Position) + Joints.JointsArray[joint->ParentIndex].Position;
    }
}
void CAnimatedMesh::update_vertex_transform()
{
    u32 AnimatedMeshUnitsCount = AnimatedMeshUnits.size();
    for(u32 mun = 0; mun < AnimatedMeshUnitsCount; mun++)
    {
        IVertexWeightBuffer* VertexWeightBuffer = AnimatedMeshUnits[mun].WeightBuffer;
        IVertexBuffer*       VertexBuffer       = AnimatedMeshUnits[mun].VertexBuffer;

        u32 SkinnedVertexCount = VertexWeightBuffer->WeightBuffer.size();

//        if(SkinnedVertexCount > VertexBuffer->getVertexCount())
//            VertexBuffer->getVerticesArray()->reserve(SkinnedVertexCount - VertexBuffer->getVertexCount());

        for(u32 swn = 0; swn < SkinnedVertexCount; swn++)
        {
            SVertexWeight& SkinnedVertex  = VertexWeightBuffer->WeightBuffer[swn];
            SVertex        Vertex         ;//= VertexBuffer->getVertex(SkinnedVertex.VertexIndex);

            core::vector3f VertexPos(0,0,0);

            u32 VertexWeightsCount = SkinnedVertex.Weights.size();
            for(u32 vwn = 0; vwn < VertexWeightsCount; vwn++)
            {
                SWeight& VertexWeight = SkinnedVertex.Weights[vwn];
                SJoint&        Joint  = Joints.JointsArray[VertexWeight.JointIndex];

                VertexPos += (Joint.transform_vector(VertexWeight.Position) * VertexWeight.Weight) ;
            }
            Vertex.Position = VertexPos;
        }
    }
}
}
}
