#ifndef _uicallbacks_h
#define _uicallbacks_h

#include <mezzanine.h>
#include "profilemanager.h"

using namespace Mezzanine;

/// @brief Subscriber method allowing publishers to break the main loop.
/// @param Args An EventArguments struct containing additional data about the event being fired.
void AllAppExit(UI::WidgetEventPtr Args);

/// @brief Subscriber method to be used to trigger going back to the main menu from gameplay.
/// @param Args An EventArguments struct containing additional data about the event being fired.
void GSMMReturn(UI::WidgetEventPtr Args);

/// @brief Subscriber method to be used to trigger the reload/reset of the current level.
/// @param Args An EventArguments struct containing additional data about the event being fired.
void GSRestart(UI::WidgetEventPtr Args);

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a callback class for creating a new empty profile.
/// @details
///////////////////////////////////////
class MSProfileCreate
{
protected:
    /// @internal
    /// @brief A pointer to the screen where the start button belongs to.
    UI::Screen* MainScreen;
    /// @internal
    /// @brief A pointer to the profile manager that will be doing the actual creating.
    ProfileManager* ProfileMan;
public:
    /// @brief Class constructor.
    /// @param HostScreen A pointer to the screen to work with.
    MSProfileCreate(ProfileManager* Manager, UI::Screen* HostScreen);
    /// @brief Class destructor.
    virtual ~MSProfileCreate();

    /// @brief Executes subscriber specific functionality when the event is fired.
    /// @param Args The arguments that describe the fired event.
    virtual void operator()(UI::WidgetEventPtr Args);
};//MSProfileCreate

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a callback class for setting a new active profile.
/// @details
///////////////////////////////////////
class MSProfileSelect
{
protected:
    /// @internal
    /// @brief A pointer to the screen where the profile configuration is updated.
    UI::Screen* MainScreen;
    /// @internal
    /// @brief A pointer to the profile manager that will be doing the actual creating.
    ProfileManager* ProfileMan;
public:
    /// @brief Class constructor.
    /// @param HostScreen A pointer to the screen to work with.
    /// @param Manager A pointer to the ProfileManager to be used for profile operations.
    MSProfileSelect(ProfileManager* Manager, UI::Screen* HostScreen);
    /// @brief Class destructor.
    virtual ~MSProfileSelect();

    /// @brief Executes subscriber specific functionality when the event is fired.
    /// @param Args The arguments that describe the fired event.
    virtual void operator()(UI::WidgetEventPtr Args);
};//MSProfileSelect

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a callback class for updating the state of a focused level select child.
/// @details
///////////////////////////////////////
class MSLevelCellSelect
{
protected:
    /// @internal
    /// @brief A pointer to the screen where the profile configuration is updated.
    UI::Screen* MainScreen;
public:
    /// @brief Class constructor.
    /// @param HostScreen A pointer to the screen to work with.
    MSLevelCellSelect(UI::Screen* HostScreen);
    /// @brief Class destructor.
    virtual ~MSLevelCellSelect();

    /// @brief Executes subscriber specific functionality when the event is fired.
    /// @param Args The arguments that describe the fired event.
    virtual void operator()(UI::WidgetEventPtr Args);
};//MSLevelCellSelect

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a callback class for launching a level from the main menu.
/// @details
///////////////////////////////////////
class MSLevelStart
{
protected:
    /// @internal
    /// @brief A pointer to the grid container storing the UI representation of the levels.
    UI::GridContainer* LevelSelect;
public:
    /// @brief Class constructor.
    /// @param LS A pointer to the grid container storing the UI representation of the levels.
    MSLevelStart(UI::GridContainer* LS);
    /// @brief Class destructor.
    virtual ~MSLevelStart();

    /// @brief Executes subscriber specific functionality when the event is fired.
    /// @param Args The arguments that describe the fired event.
    virtual void operator()(UI::WidgetEventPtr Args);
};//MSLevelStart

#endif
