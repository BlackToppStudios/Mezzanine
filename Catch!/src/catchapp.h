#ifndef _catchapp_h
#define _catchapp_h

#include "startingarea.h"
#include "scorearea.h"
#include "areaofplay.h"
#include "levelloader.h"
#include <physgame.h>

using namespace phys;

class CatchApp
{
    public:
        enum GameState
        {
            Catch_Init,
            Catch_Loading,
            Catch_MenuScreen,
            Catch_GameScreen,
            Catch_Paused
        };
    protected:
        static CatchApp* TheRealCatchApp;
        const Plane PlaneOfPlay;
        World* TheWorld;
        LevelLoader* Loader;
        ActorBase* LastActorThrown;
        AreaOfPlay* PlayZone;
        Whole CurrScore;
        CatchApp::GameState CurrentState;
        std::map<String,Whole> ItemScoreValues;
        std::map<String,Whole> ShopCostValues;
        std::vector<ActorBase*> ThrownItems;
        std::vector<StartingArea*> StartAreas;
        std::vector<ScoreArea*> ScoreAreas;

        SimpleTimer* LevelTimer;
        SimpleTimer* EndTimer;

        void MakeGUI();
        void CreateLoadingScreen();
        void ConfigResources();
        void InitMusic();

        void PopulateScoreValues();
        void PopulateShopValues();
        void PopulateLevelList(UI::PagedCellGrid* Grid);
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

        //Callbacks AKA Functions the mainloops call
        bool PreInput();
        bool PostInput();
        bool PreUI();
        bool PostUI();
        bool PrePhysics();
        bool PostPhysics();
        bool PostRender();

        bool CheckForStuff();
        void SetPlayArea(AreaOfPlay* PlayArea);
        void RegisterScoreArea(ScoreArea* Score);
        void RegisterStartArea(StartingArea* Start);
        void AddThrowable(ActorBase* Throwable);
        LevelLoader* GetLevelLoader();
        SimpleTimer* GetLevelTimer();
        SimpleTimer* GetEndTimer();
};

#endif
