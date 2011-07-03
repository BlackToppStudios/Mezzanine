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
        static std::vector<ThrowableData*> Throwables;
    public:
        ThrowableGenerator();
        ~ThrowableGenerator();
        static ThrowableGenerator* GetThrowableGenerator();
        static void ParseThrowables(const String& FileName);
        static ThrowableData* GetThrowableData(const String& Throwable);
        static ActorRigid* CreateThrowable(const String& Throwable);
};

#endif
