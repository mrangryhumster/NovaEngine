#ifndef IANIMATEDMESH_H
#define IANIMATEDMESH_H

#include "IStaticMesh.h"

namespace novaengine
{
namespace renderer
{

class IVertexWeightBuffer;

struct SJoint
{
    core::string Name;

    s32          ParentIndex;

    core::quaternionf   Rotation;
    core::vector3f      Position;

    bool            TransformUpdated;
    void update_transform()
    {
        //Position = Rotation.rotate_vector(Position) + Position
    }

    core::vector3f transform_vector(core::vector3f Vector)
    {
        return Rotation.rotate_vector(Vector) + Position;
    }
};

struct SJoints
{
    int getJointIndexByName(const core::string& joint_name)
    {
        u32 JointsArraySize = JointsArray.size();
        for(u32 it = 0; it < JointsArraySize;it++)
            if(joint_name.compare(JointsArray[it].Name) == 0)
                return it;
        return -1;
    }
    std::vector<SJoint> JointsArray;
};

struct SWeight
{
    u32             JointIndex;
    core::vector3f  Position;
    f32             Weight;
};

struct SVertexWeight
{
    u32                  VertexIndex;
    std::vector<SWeight> Weights;
};

struct SAnimatedMeshUnit
{
    SAnimatedMeshUnit():
        VertexBuffer(NULL),
        WeightBuffer(NULL),
        Material(NULL)
        {}

    IVertexBuffer*              VertexBuffer;
    IVertexWeightBuffer*        WeightBuffer;
    IMaterial*                  Material;
};

class IVertexWeightBuffer : public Object
{
public:
    std::vector<SVertexWeight> WeightBuffer;

};

struct SJointTransformation
{
    u32 JointIndex;
    core::vector3f    Position;
    core::quaternionf Rotation;
};
struct SAnimationKeyFrame
{
    f64 TimePosition;
    std::vector<SJointTransformation> Transformations;
};
struct SAnimation
{
    core::string    Name;
    f64             Lenght;
    std::vector<SAnimationKeyFrame>     KeyFrames;
};

class IAnimatedMesh : public IStaticMesh
{
    public:

        virtual void addAnimatedMeshUnit(SAnimatedMeshUnit) = 0;
        virtual void addAnimatedMeshUnit(IVertexBuffer*,IVertexWeightBuffer*,IMaterial*) = 0;

        virtual const SMeshUnit*  getMeshUnit(u32 id)          = 0;
        virtual SAnimatedMeshUnit getAnimatedMeshUnit(u32 id)  = 0;

        virtual void     setJoints(SJoints) = 0;
        virtual SJoints* getJoints() = 0;

        //virtual SAnimation getAnimationCount()        = 0;
        virtual void       addAnimation(SAnimation)   = 0;

        virtual void applyAnimationByTime( f32 Time)  = 0;
        virtual void applyAnimationByFrame(u32 Frame) = 0;
        virtual void applyAnimationByTime( f32 StartTime,f32 EndTime,f32 DeltaTime)   = 0;
        virtual void applyAnimationByFrame(u32 StartFrame,u32 EndFrame,f32 DeltaTime) = 0;


    protected:
    private:

        virtual void addMeshUnit(SMeshUnit )                = 0;
        virtual void addMeshUnit(IVertexBuffer*,IMaterial*) = 0;
};

}
}
#endif // IANIMATEDMESH_H
