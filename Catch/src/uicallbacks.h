#ifndef _uicallbacks_h
#define _uicallbacks_h

#include <mezzanine.h>

using namespace Mezzanine;

/// @brief Subscriber method allowing publishers to break the main loop.
/// @param Args An EventArguments struct containing additional data about the event being fired.
void AllAppExit(EventArgumentsPtr Args);

/// @brief Subscriber method to be used to trigger going back to the main menu from gameplay.
/// @param Args An EventArguments struct containing additional data about the event being fired.
void GSMMReturn(EventArgumentsPtr Args);

/// @brief Subscriber method to be used to trigger the reload/reset of the current level.
/// @param Args An EventArguments struct containing additional data about the event being fired.
void GSRestart(EventArgumentsPtr Args);

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a callback class for updating the state of a focused level select child.
/// @details
///////////////////////////////////////
class MSLevelCellSelect : public FunctorEventSubscriber
{
protected:
    /// @internal
    /// @brief A pointer to the screen where the start button belongs to.
    UI::Screen* LSScreen;
public:
    /// @brief Class constructor.
    /// @param HostScreen A pointer to the screen to work with.
    MSLevelCellSelect(UI::Screen* HostScreen);
    /// @brief Class destructor.
    virtual ~MSLevelCellSelect();

    /// @brief Executes subscriber specific functionality when the event is fired.
    /// @param Args The arguments that describe the fired event.
    virtual void operator()(EventArgumentsPtr Args);
};//MSLevelCellSelect

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a callback class for launching a level from the main menu.
/// @details
///////////////////////////////////////
class MSLevelStart : public FunctorEventSubscriber
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
    virtual void operator()(EventArgumentsPtr Args);
};//MSLevelStart

#endif
