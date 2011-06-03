#ifndef _throwablegenerator_h
#define _throwablegenerator_h

#include <physgame.h>

using namespace phys;

struct ThrowableData
{
    Whole ThrowableCount;
    Real Mass;
    String ThrowableName;
    String MeshName;
    String GroupName;
};

class ThrowableGenerator
{
    protected:
        static ThrowableGenerator* TheRealThrowableGenerator;
    public:
        ThrowableGenerator();
        ~ThrowableGenerator();
        static ThrowableGenerator* GetThrowableGenerator();
};

#endif
