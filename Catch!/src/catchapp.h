#ifndef _catchapp_h
#define _catchapp_h

#include "startingarea.h"
#include "scorearea.h"
#include <physgame.h>

using namespace phys;

class CatchApp
{
    public:
        enum GameState
        {
            Catch_Loading,
            Catch_MenuScreen,
            Catch_GameScreen,
            Catch_Paused
        };
    protected:
        static CatchApp* TheRealCatchApp;
        const Plane PlaneOfPlay;
        World* TheWorld;
        ActorBase* LastActorThrown;
        StartingArea* StartZone;
        ScoreArea* ScoreZone;
        Whole CurrScore;
        CatchApp::GameState CurrentState;

        void LoadContent();
        void MakeGUI();
    public:
        CatchApp(const Vector3 &WorldLowerBounds, const Vector3 &WorldUpperBounds, SceneManager::SceneManagerType SceneType, const unsigned short int &MaxPhysicsProxies);
        ~CatchApp();

        static CatchApp* GetCatchAppPointer();
        int GetCatchin();

        //Callbacks AKA Functions the mainloops call
        bool PreInput();
        bool PostInput();
        bool PreUI();
        bool PostUI();
        bool PrePhysics();
        bool PostPhysics();
        bool PostRender();

        bool CheckForEsc();
};

#endif
