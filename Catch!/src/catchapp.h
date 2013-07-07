#ifndef _catchapp_h
#define _catchapp_h

#include "levelzones.h"
#include "levelloader.h"
#include "levelscorer.h"
#include "itemshop.h"
#include "profilemanager.h"
#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp
{
    public:
        enum GameState
        {
            Catch_Init,
            Catch_Loading,
            Catch_MenuScreen,
            Catch_GameScreen,
            Catch_ScoreScreen
        };
    protected:
        static CatchApp* TheRealCatchApp;

        Entresol* TheEntresol;
        ProfileManager* Profiles;
        LevelLoader* Loader;
        LevelScorer* Scorer;
        ItemShop* Shop;
        ActorBase* LastActorThrown;

        Timer* LevelTimer;
        StopWatchTimer* EndTimer;

        bool Paused;
        Whole CurrScore;
        CatchApp::GameState CurrentState;

        const Plane PlaneOfPlay;

        std::vector<ActorBase*> ThrownItems;
        std::vector<StartArea*> StartAreas;
        std::vector<ScoreArea*> ScoreAreas;

        void MakeGUI();
        void CreateLoadingScreen();
        void InitMusic();
        void VerifySettings();

        void ChangeState(const CatchApp::GameState &StateToSet);

        bool CheckEndOfLevel();
        bool AllStartZonesEmpty();
        bool IsInsideAnyStartZone(ActorBase* Actor);
        void UnloadLevel();
    public:
        CatchApp();
        ~CatchApp();

        static CatchApp* GetCatchAppPointer();
        int GetCatchin();
        void PauseGame(bool Pause);
        bool GameIsPaused();

        //Callbacks AKA Functions the mainloops call
        //bool PreInput();
        bool PostInput();
        bool PreUI();
        bool PostUI();
        bool PrePhysics();
        bool PostPhysics();
        bool PostRender();

        bool CheckForStuff();
        bool IsAThrowable(ActorBase* Actor);
        void RegisterScoreArea(ScoreArea* Score);
        void RegisterStartArea(StartArea* Start);
        void AddThrowable(ActorBase* Throwable);
        std::vector<ActorBase*>& GetThrowables();
        std::vector<ScoreArea*>& GetScoreAreas();
        LevelLoader* GetLevelLoader();
        LevelScorer* GetLevelScorer();
        ProfileManager* GetProfiles();
        ItemShop* GetItemShop();
        Timer* GetLevelTimer();
        StopWatchTimer* GetEndTimer();
};//CatchApp

#endif
