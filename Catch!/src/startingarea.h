#ifndef _startingarea_h
#define _startingarea_h

#include <areaeffect.h>

using namespace phys;

class StartingArea : public AreaEffect
{
    protected:
        Vector3 Grav;
    public:
        StartingArea(const String &name, Vector3 Location);
        ~StartingArea();
        void Initialize();
        void ApplyEffect();
        bool IsEmpty();
};

#endif
