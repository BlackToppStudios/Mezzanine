#ifndef _uicallbacks_h
#define _uicallbacks_h

#include <mezzanine.h>

using namespace Mezzanine;

/// @brief Subscriber method allowing publishers to break the main loop.
/// @param Args An EventArguments struct containing additional data about the event being fired.
void AllAppExit(EventArgumentsPtr Args);

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

#endif
