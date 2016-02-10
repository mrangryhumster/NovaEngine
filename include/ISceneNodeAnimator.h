#ifndef ISCENENODEANIMATOR_H
#define ISCENENODEANIMATOR_H


#include "NTypes.h"
#include "Object.h"
#include "ISceneNode.h"

namespace novaengine
{
namespace scene
{

class ISceneNodeAnimator : public Object
{
    public:
        virtual void setActive() = 0;
        virtual bool isActive()  = 0;

        virtual u32  getID() = 0;
        virtual void animate(ISceneNode* AnimatedNode,f32 DeltaTime) = 0;
    protected:
    private:
};

}
}

#endif // ISCENENODEANIMATOR_H
