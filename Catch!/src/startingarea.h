#ifndef _startingarea_h
#define _startingarea_h

#include <areaeffect.h>

using namespace phys;

class StartingArea : public AreaEffect
{
    protected:
        bool Init;
    public:
        StartingArea(const String &name, Vector3 Location);
        virtual ~StartingArea();
        virtual void ApplyEffect();
        virtual bool IsInside(ActorBase* Actor);
        virtual bool IsEmpty();
};

#endif
