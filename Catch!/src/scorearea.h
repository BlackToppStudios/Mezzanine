#ifndef _scorearea_h
#define _scorearea_h

#include <areaeffect.h>

using namespace phys;

class ScoreArea : public AreaEffect
{
    protected:
    public:
        ScoreArea(const String &name, Vector3 Location, World* world);
        ~ScoreArea();
        void ApplyEffect();
        bool AllObjectsAtRest();
};

#endif
