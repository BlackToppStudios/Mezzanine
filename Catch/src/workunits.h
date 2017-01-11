#ifndef _workunits_h
#define _workunits_h

#include <mezzanine.h>
#include "queuedsubscriber.h"

using namespace Mezzanine;

class CatchApp;

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible fullfilling an Audio settings change requested from the UI.
///////////////////////////////////////
class AudioSettingsWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief The subscriber used to store settings change requests until they can be processed.
    QueuedSettingsSubscriber* SettingsSubscriber;
public:
    /// @brief Class constructor.
    /// @param UITarget The UIManager to be used for widget lookups.
    /// @param Target The target manager this work unit will alter.
    AudioSettingsWorkUnit(UI::UIManager* UITarget, ManagerBase* Target);
    /// @brief Class destructor.
    virtual ~AudioSettingsWorkUnit();

    /// @brief Gets a pointer to the subscriber used to queue settings change requests.
    /// @return Returns a pointer to the subscriber storing settings change requests.
    QueuedSettingsSubscriber* GetSettingsSubscriber() const;
    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};//AudioSettingsWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible fullfilling an Video settings change requested from the UI.
///////////////////////////////////////
class VideoSettingsWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief The subscriber used to store settings change requests until they can be processed.
    QueuedSettingsSubscriber* SettingsSubscriber;
public:
    /// @brief Class constructor.
    /// @param UITarget The UIManager to be used for widget lookups.
    /// @param Target The target manager this work unit will alter.
    VideoSettingsWorkUnit(UI::UIManager* UITarget, ManagerBase* Target);
    /// @brief Class destructor.
    virtual ~VideoSettingsWorkUnit();

    /// @brief Gets a pointer to the subscriber used to queue settings change requests.
    /// @return Returns a pointer to the subscriber storing settings change requests.
    QueuedSettingsSubscriber* GetSettingsSubscriber() const;
    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};//VideoSettingsWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible for all Catch logic to be run before Input is collected.
///////////////////////////////////////
class CatchPreInputWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief A pointer to the Catch core class.
    CatchApp* CatchApplication;
public:
    /// @brief Class constructor.
    /// @param Target A pointer to the Catch core class.
    CatchPreInputWorkUnit(CatchApp* Target);
    /// @brief Class destructor.
    virtual ~CatchPreInputWorkUnit();

    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// CatchPreInputWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible for all Catch logic to be run after Input is collected.
///////////////////////////////////////
class CatchPostInputWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief A pointer to the Catch core class.
    CatchApp* CatchApplication;
public:
    /// @brief Class constructor.
    /// @param Target A pointer to the Catch core class.
    CatchPostInputWorkUnit(CatchApp* Target);
    /// @brief Class destructor.
    virtual ~CatchPostInputWorkUnit();

    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// CatchPostInputWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible for all Catch logic to be run after UI widgets are updated.
/// @details
///////////////////////////////////////
class CatchPostUIWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief A pointer to the Catch core class.
    CatchApp* CatchApplication;
    /// @internal
    /// @brief The raycaster used for click and drag in Catch.
    RayQueryTool RayCaster;
public:
    /// @brief Class constructor.
    /// @param Target A pointer to the Catch core class.
    CatchPostUIWorkUnit(CatchApp* Target);
    /// @brief Class destructor.
    virtual ~CatchPostUIWorkUnit();

    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// CatchPostUIWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible for fullfilling a request to pause the game.
/// @details
///////////////////////////////////////
class CatchPauseWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief The subscriber used to pause state change requests until they can be processed.
    QueuedPauseSubscriber* PauseSubscriber;
public:
    /// @brief Class constructor.
    /// @param Target A pointer to the Catch core class.
    /// @param UITarget The UIManager to be used for widget lookups.
    CatchPauseWorkUnit(CatchApp* Target, UI::UIManager* UITarget);
    /// @brief Class destructor.
    virtual ~CatchPauseWorkUnit();

    /// @brief Gets a pointer to the subscriber used to queue pause change requests.
    /// @return Returns a pointer to the subscriber storing pause change requests.
    QueuedPauseSubscriber* GetPauseSubscriber() const;
    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};//CatchPauseWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible for updating the in-game HUD information.
/// @details
///////////////////////////////////////
class CatchHUDUpdateWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief A pointer to the Catch core class.
    CatchApp* CatchApplication;
public:
    /// @brief Class constructor.
    /// @param Target A pointer to the Catch core class.
    CatchHUDUpdateWorkUnit(CatchApp* Target);
    /// @brief Class destructor.
    virtual ~CatchHUDUpdateWorkUnit();

    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// CatchHUDUpdateWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible for checking if the events need to end a level have occured.
/// @details
///////////////////////////////////////
class CatchEndLevelWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief A pointer to the Catch core class.
    CatchApp* CatchApplication;
public:
    /// @brief Class constructor.
    /// @param Target A pointer to the Catch core class.
    CatchEndLevelWorkUnit(CatchApp* Target);
    /// @brief Class destructor.
    virtual ~CatchEndLevelWorkUnit();

    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// CatchEndLevelWorkUnit

#endif
