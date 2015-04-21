#ifndef CKEYMAP_H
#define CKEYMAP_H

#include <map>
namespace novaengine
{
namespace window
{
//! This class need to map keyevents from window to novaengine::sevent::keyboard::keycode
template<class KEY_OS,class KEY_ENGINE>
class CKeyMap
{
    public:
        void add(KEY_OS os_key,KEY_ENGINE engine_key)
        {
            keymap[os_key] = engine_key;
        }
        KEY_ENGINE get(KEY_OS os_key)
        {
            return  keymap[os_key];
        }
        void clear()
        {
            keymap.clear();
        }
    protected:
    private:
        std::map<KEY_OS,KEY_ENGINE> keymap;
};
}
}
#endif // CKEYMAP_H
