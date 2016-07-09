#ifndef IGUILAYOUT_H
#define IGUILAYOUT_H

#include "Object.h"

namespace novaengine
{
namespace gui
{
class IGUINode;

class IGUILayout : public Object
{
    public:

    //NodeList
    virtual void RegisterNode(IGUINode*  ,bool allow_callback = true) = 0;
    virtual void UnRegisterNode(IGUINode*,bool allow_callback = true) = 0;

    virtual IGUINode** getNodeList()       = 0;
    virtual u32        getNodeCount()      = 0;
    virtual void       eraseScene()        = 0;
    virtual IGUINode*  getNodeByID(s32 id) = 0;

    //Layout
    virtual void setActive() = 0;
    virtual bool isActive()  = 0;

    //Render
    virtual void update()     = 0;
    virtual void animate(f32) = 0;
    virtual void render()     = 0;


    protected:
    private:
};

}
}

#endif // IGUILAYOUT_H
