#ifndef _startingarea_h
#define _startingarea_h

#include <areaeffect.h>

using namespace phys;

class StartingArea : public AreaEffect
{
    protected:
    public:
        StartingArea(const String &name, Vector3 Location);
        ~StartingArea();
        void ApplyEffect();
        bool IsInside(ActorBase* Actor);
        bool IsEmpty();
};

#endif
