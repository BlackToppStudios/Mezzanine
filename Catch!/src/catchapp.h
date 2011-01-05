#ifndef _catchapp_h
#define _catchapp_h

#include <physgame.h>

using namespace phys;

class CatchApp
{
    protected:
        static CatchApp* TheRealCatchApp;
        World* GameWorld;
    public:
        CatchApp();
        ~CatchApp();
        static CatchApp* GetCatchAppPointer();
};

#endif
