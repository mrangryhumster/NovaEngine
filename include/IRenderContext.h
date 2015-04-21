#ifndef IRENDERCONTEXT_H
#define IRENDERCONTEXT_H

#include "Object.h"

namespace novaengine
{
namespace renderer
{
class IRenderContext : public Object
{
    public:

        virtual bool create_context() = 0;
        virtual void make_current() = 0;
        virtual void delete_context() = 0;
        virtual void flush() = 0;

        virtual void setVSync(bool) = 0;
        virtual bool getVSync()     = 0;

        virtual bool isOk() = 0;

    protected:
    private:
};
}
}
#endif // IRENDERCONTEXT_H
