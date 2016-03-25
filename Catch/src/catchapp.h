#ifndef _catchapp_h
#define _catchapp_h

#include <mezzanine.h>

#include "WorldObjects/scorearea.h"
#include "WorldObjects/startarea.h"
#include "levelmanager.h"
#include "levelscorer.h"
#include "itemshop.h"
#include "profilemanager.h"
#include "workunits.h"

using namespace Mezzanine;

class CatchApp
{
public:
    typedef std::vector<Debris*> ThrowableContainer;
    typedef std::vector<StartArea*> StartAreaContainer;
    typedef std::vector<ScoreArea*> ScoreAreaContainer;
    enum GameState
    {
        Catch_Init,
        Catch_Loading,
        Catch_MenuScreen,
        Catch_GameScreen,
        Catch_ScoreScreen
    };
protected:
    friend class CatchPreInputWorkUnit;
    friend class CatchPostInputWorkUnit;
    friend class CatchPostUIWorkUnit;
    friend class CatchHUDUpdateWorkUnit;
    friend class CatchEndLevelWorkUnit;

    static CatchApp* TheRealCatchApp;

    AudioSettingsWorkUnit* AudioSettingsWork;
    VideoSettingsWorkUnit* VideoSettingsWork;
    CatchPreInputWorkUnit* PreInputWork;
    CatchPostInputWorkUnit* PostInputWork;
    CatchPostUIWorkUnit* PostUIWork;
    CatchPauseWorkUnit* PauseWork;
    CatchHUDUpdateWorkUnit* HUDUpdateWork;
    CatchEndLevelWorkUnit* EndLevelWork;
    Scripting::Lua::Lua51WorkUnit* LuaScriptWork;

    Entresol* TheEntresol;
    World* TheWorld;
    ProfileManager* Profiles;
    LevelManager* LevelMan;
    LevelScorer* Scorer;
    ItemShop* Shop;
    Debris* LastObjectThrown;

    Timer* LevelTimer;
    Timer* EndTimer;

    Boole Paused;
    CatchApp::GameState CurrentState;

    const Plane PlaneOfPlay;

    ThrowableContainer ThrownItems;
    StartAreaContainer StartAreas;

    void InitializeFromXML(const String& CatchDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile);

    void CreateWorld();
    void MakeGUI();
    void CreateLoadingScreen();
    void InitMusic();
    void VerifySettings();
    void RegisterTypes();

    void ChangeState(const CatchApp::GameState StateToSet);

    Boole CheckEndOfLevel();
    Boole AllStartZonesEmpty();
    void UnloadLevel();
public:
    CatchApp();
    ~CatchApp();

    static CatchApp* GetCatchAppPointer();
    int GetCatchin();
    CatchApp::GameState GetState() const;
    void PauseGame(Boole Pause);
    Boole GameIsPaused() const;

    void SetVisibleScreens(const CatchApp::GameState State);

    Boole IsAThrowable(WorldObject* Throwable) const;
    Boole IsInsideAnyStartZone(Debris* Throwable) const;
    void RegisterStartArea(StartArea* Start);
    void AddThrowable(Debris* Throwable);

    AudioSettingsWorkUnit* GetAudioSettingsWork() const;
    VideoSettingsWorkUnit* GetVideoSettingsWork() const;
    CatchPreInputWorkUnit* GetPreInputWork() const;
    CatchPostInputWorkUnit* GetPostInputWork() const;
    CatchPostUIWorkUnit* GetPostUIWork() const;
    CatchPauseWorkUnit* GetPauseWork() const;
    CatchHUDUpdateWorkUnit* GetHUDUpdateWork() const;
    CatchEndLevelWorkUnit* GetEndLevelWork() const;
    Scripting::Lua::Lua51WorkUnit* GetLuaScriptWork() const;

    Entresol* GetTheEntresol() const;
    World* GetTheWorld() const;

    ThrowableContainer& GetThrowables();
    LevelManager* GetLevelManager() const;
    LevelScorer* GetLevelScorer() const;
    ProfileManager* GetProfiles() const;
    ItemShop* GetItemShop() const;
    Timer* GetLevelTimer() const;
    Timer* GetEndTimer() const;
};//CatchApp

#endif
