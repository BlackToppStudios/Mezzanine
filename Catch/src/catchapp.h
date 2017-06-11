#ifndef _catchapp_h
#define _catchapp_h

#include <mezzanine.h>

#include "WorldEntities/levelzone.h"
#include "WorldEntities/startarea.h"
#include "levelmanager.h"
#include "levelscorer.h"
#include "itemshop.h"
#include "catchplayer.h"
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
    friend class CatchPostInputWorkUnit;
    friend class CatchPostUIWorkUnit;
    friend class CatchHUDUpdateWorkUnit;
    friend class CatchEndLevelWorkUnit;

    static CatchApp* TheRealCatchApp;

    AudioSettingsWorkUnit* AudioSettingsWork;
    VideoSettingsWorkUnit* VideoSettingsWork;
    CatchPostInputWorkUnit* PostInputWork;
    CatchPostUIWorkUnit* PostUIWork;
    CatchPauseWorkUnit* PauseWork;
    CatchHUDUpdateWorkUnit* HUDUpdateWork;
    CatchEndLevelWorkUnit* EndLevelWork;
    Scripting::Lua::Lua51WorkUnit* LuaScriptWork;

    /// @brief A pointer to the root engine object simulating the game.
    Entresol* TheEntresol;
    /// @brief A pointer to the game world.
    World* TheWorld;
    ProfileManager* Profiles;
    LevelManager* LevelMan;
    LevelScorer* Scorer;
    ItemShop* Shop;

    Debris* LastObjectThrown;
    /// @brief A pointer to the player.
    CatchPlayer* ThePlayer;

    Timer* LevelTimer;
    Timer* EndTimer;

    Boole Paused;
    CatchApp::GameState CurrentState;

    MousePicker Picker;
    MousePicker::FilterDelegate PickerFilter;

    ThrowableContainer ThrownItems;
    StartAreaContainer StartAreas;

    AppEventDispatcher AppEvents;

    void InitializeFromXML(const String& CatchDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile);

    /// @brief Creates a game world ready to be populated.
    void CreateWorld();
    /// @brief Sets up the game HUD/Menus.
    void MakeGUI();
    /// @brief Sets up the loading screen for the game.
    void CreateLoadingScreen();
    /// @brief Loads all the music and sets up a playlist.
    void InitMusic();
    /// @brief Verifies all manager and player settings by loading from files or making defaults.
    void VerifySettings();
    /// @brief Registers all custom game factories with the engine.
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

    Boole IsAThrowable(WorldEntity* Throwable) const;
    Boole IsInsideAnyStartZone(Debris* Throwable) const;
    void RegisterStartArea(StartArea* Start);
    void AddThrowable(Debris* Throwable);

    AudioSettingsWorkUnit* GetAudioSettingsWork() const;
    VideoSettingsWorkUnit* GetVideoSettingsWork() const;
    CatchPostInputWorkUnit* GetPostInputWork() const;
    CatchPostUIWorkUnit* GetPostUIWork() const;
    CatchPauseWorkUnit* GetPauseWork() const;
    CatchHUDUpdateWorkUnit* GetHUDUpdateWork() const;
    CatchEndLevelWorkUnit* GetEndLevelWork() const;
    Scripting::Lua::Lua51WorkUnit* GetLuaScriptWork() const;

    Entresol* GetTheEntresol() const;
    World* GetTheWorld() const;

    MousePicker& GetPicker();
    const MousePicker::FilterDelegate& GetPickerFilter() const;

    ThrowableContainer& GetThrowables();
    LevelManager* GetLevelManager() const;
    LevelScorer* GetLevelScorer() const;
    ProfileManager* GetProfiles() const;
    ItemShop* GetItemShop() const;
    CatchPlayer* GetPlayer() const;
    Timer* GetLevelTimer() const;
    Timer* GetEndTimer() const;
};//CatchApp

#endif
