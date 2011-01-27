#ifndef _catchapp_h
#define _catchapp_h

#include "startingarea.h"
#include "scorearea.h"
#include <physgame.h>

using namespace phys;

class CatchApp
{
    protected:
        static CatchApp* TheRealCatchApp;
        World* GameWorld;
        ActorBase* LastActorThrown;
        StartingArea* StartZone;
        ScoreArea* ScoreZone;
        Whole CurrScore;
    public:
        CatchApp(const Vector3 &WorldLowerBounds, const Vector3 &WorldUpperBounds, SceneManager::SceneManagerType SceneType, const unsigned short int &MaxPhysicsProxies);
        ~CatchApp();
        static CatchApp* GetCatchAppPointer();
};

#endif
