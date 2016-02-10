#ifndef CANIMATEDMESH_H
#define CANIMATEDMESH_H

#include "IAnimatedMesh.h"

namespace novaengine
{
namespace renderer
{

class CAnimatedMesh : public IAnimatedMesh
{
public:
    CAnimatedMesh();
    virtual ~CAnimatedMesh();


    u32  getMeshUnitsCount();

    void addAnimatedMeshUnit(SAnimatedMeshUnit);
    void addAnimatedMeshUnit(IVertexBuffer*,IVertexWeightBuffer*,IMaterial*);

    const SMeshUnit*  getMeshUnit(u32 id);
    SAnimatedMeshUnit getAnimatedMeshUnit(u32 id);


    void removeMeshUnit(u32 id);

    IVertexBuffer* getVertexBuffer(u32 id);
    IMaterial* getMaterial(u32 id);

    void setVertexBuffer(u32 id,IVertexBuffer*) {};
    void setMaterial(u32 id,IMaterial*)         {};

    IStaticMesh* getCopy();

    void setJoints(SJoints);
    SJoints* getJoints();

    u32  getAnimationFrameCount();
    void addAnimation(SAnimation);

    void applyAnimationByTime (f32 Time);
    void applyAnimationByFrame(u32 Frame);
    void applyAnimationByTime (f32 StartTime,f32 EndTime,f32 DeltaTime);
    void applyAnimationByFrame(u32 StartFrame,u32 EndFrame,f32 DeltaTime);


protected:
private:
    void update_joint_tranform(SJoint* joint);
    void update_vertex_transform();
    //--------------------------------------------------------------------
    void addMeshUnit(SMeshUnit MeshUnit){};
    void addMeshUnit(IVertexBuffer* VertexBuffer,IMaterial* Material){};
    //--------------------------------------------------------------------

    u32                            AnimatedMeshUnitsCount;
    SAnimation                     Animation;
    std::vector<SAnimatedMeshUnit> AnimatedMeshUnits;
    //std::vector<SJoint>            AnimationJoints;
    SJoints                        Joints;
};

}
}

#endif // CANIMATEDMESH_H
