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
        virtual void setActive(bool state)
        {
            AnimatorEnabled = state;
        }
        virtual bool isActive()
        {
            return AnimatorEnabled;
        }

        virtual u32  getID()
        {
            return AnimatorID;
        }

        virtual void animate(ISceneNode* AnimatedNode,f32 DeltaTime) = 0;

    protected:
    private:

    u32  AnimatorID;
    bool AnimatorEnabled;
};

}
}

#endif // ISCENENODEANIMATOR_H
