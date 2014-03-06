#ifndef _workunits_h
#define _workunits_h

#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp;

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a convenience class used by some of the settings related work units.
/// @details
///////////////////////////////////////
class QueuedSubscriber : public EventSubscriber
{
public:
    /// @brief Container type for EventArguements storage in this class.
    typedef std::vector<EventArgumentsPtr>        EventContainer;
    /// @brief Iterator type for EventArguments stored in this class.
    typedef EventContainer::iterator              EventIterator;
    /// @brief Const Iterator type for EventArguments stored in this class.
    typedef EventContainer::const_iterator        ConstEventIterator;
protected:
public:
    /// @brief Container storing all of the Queued events.
    EventContainer Events;
    /// @brief A pointer to the manager that this events will eventually change the settings on.
    ManagerBase* TargetManager;
    /// @brief A pointer to the UIManager to give this access to the subscribed widgets.
    UI::UIManager* UITarget;
    /// @brief Stores whether or not this subscribers events are currently being processed.
    Boole Updating;

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    /// @brief Class constructor.
    /// @param UIMan A pointer to the UIManager used for Widget lookups if needed.
    /// @param Target The target manager this subscriber will alter.
    QueuedSubscriber(UI::UIManager* UIMan, ManagerBase* Target);
    /// @brief Class destructor.
    virtual ~QueuedSubscriber();

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience and Utility

    /// @brief Gets an iterator to the first event.
    /// @return Returns an Iterator pointing to the first event queued.
    ConstEventIterator GetFirstEvent() const;
    /// @brief Gets an iterator to one passed the last event.
    /// @return Returns an Iterator pointing to one passed the last event queued.
    ConstEventIterator GetEndEvent() const;
    /// @brief Clears the queue of all known events.
    void ClearEvents();

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited

    /// @brief Method used to notify this Subscriber an event it cares about has occured.
    /// @param Args The specific information regarding the event that was fired.
    virtual void _NotifyEvent(EventArgumentsPtr Args);
};//QueuedSubscriber

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible fullfilling an Audio settings change requested from the UI.
/// @details
///////////////////////////////////////
class AudioSettingsWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief The subscriber used to store settings change requests until they can be processed.
    QueuedSubscriber* SettingsSubscriber;
public:
    /// @brief Class constructor.
    /// @param UITarget The UIManager to be used for widget lookups.
    /// @param Target The target manager this work unit will alter.
    AudioSettingsWorkUnit(UI::UIManager* UITarget, ManagerBase* Target);
    /// @brief Class destructor.
    virtual ~AudioSettingsWorkUnit();

    /// @brief Gets a pointer to the subscriber used to queue settings change requests.
    /// @return Returns a pointer to the subscriber storing settings change requests.
    QueuedSubscriber* GetSettingsSubscriber() const;
    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};//AudioSettingsWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible fullfilling an Video settings change requested from the UI.
/// @details
///////////////////////////////////////
class VideoSettingsWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief The subscriber used to store settings change requests until they can be processed.
    QueuedSubscriber* SettingsSubscriber;
public:
    /// @brief Class constructor.
    /// @param UITarget The UIManager to be used for widget lookups.
    /// @param Target The target manager this work unit will alter.
    VideoSettingsWorkUnit(UI::UIManager* UITarget, ManagerBase* Target);
    /// @brief Class destructor.
    virtual ~VideoSettingsWorkUnit();

    /// @brief Gets a pointer to the subscriber used to queue settings change requests.
    /// @return Returns a pointer to the subscriber storing settings change requests.
    QueuedSubscriber* GetSettingsSubscriber() const;
    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};//VideoSettingsWorkUnit

///////////////////////////////////////////////////////////////////////////////
/// @brief This workunit is responsible for all Catch logic to be run before Input is collected.
/// @details
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
/// @details
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
/// @brief This workunit is responsible for all Catch logic to be run after the frame is rendered.
/// @details
///////////////////////////////////////
class CatchPostGraphicsWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    /// @internal
    /// @brief A pointer to the Catch core class.
    CatchApp* CatchApplication;
public:
    /// @brief Class constructor.
    /// @param Target A pointer to the Catch core class.
    CatchPostGraphicsWorkUnit(CatchApp* Target);
    /// @brief Class destructor.
    virtual ~CatchPostGraphicsWorkUnit();

    /// @brief Performs all the tasks this WorkUnit is responsible for.
    /// @param CurrentThreadStorage Storage for a number of utilities and data that is safe for this WorkUnit to access.
    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// CatchPostGraphicsWorkUnit

#endif
