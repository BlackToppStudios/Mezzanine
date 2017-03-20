#ifndef _queuedsubscriber_h
#define _queuedsubscriber_h

#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp;

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a base class for event subscribers that hold on to thier recieved events for later processing.
/// @details
///////////////////////////////////////
class CatchQueuedSubscriber
{
public:
    /// @brief Container type for EventArguements storage in this class.
    typedef std::vector<EventPtr>                 EventContainer;
    /// @brief Iterator type for EventArguments stored in this class.
    typedef EventContainer::iterator              EventIterator;
    /// @brief Const Iterator type for EventArguments stored in this class.
    typedef EventContainer::const_iterator        ConstEventIterator;
protected:
public:
    /// @brief Container storing all of the Queued events.
    EventContainer Events;
    /// @brief Stores whether or not this subscribers events are currently being processed.
    Boole Updating;

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    /// @brief Class constructor.
    CatchQueuedSubscriber();
    /// @brief Class destructor.
    virtual ~CatchQueuedSubscriber();

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience and Utility

    /// @brief Notifies this Subscriber that the events are being consumed and it is not to recieve any more events.
    void StartUpdate();
    /// @brief Notifies this Subscriber that updating is over and it is now safe to recieve events again.
    void EndUpdate();

    /// @brief Gets an iterator to the first event.
    /// @return Returns an Iterator pointing to the first event queued.
    ConstEventIterator GetFirstEvent() const;
    /// @brief Gets an iterator to one passed the last event.
    /// @return Returns an Iterator pointing to one passed the last event queued.
    ConstEventIterator GetEndEvent() const;
    /// @brief Clears the queue of all known events.
    void ClearEvents();

    /// @brief Gets a usable delegate that can be used to subscribe this to an EventPublisher.
    /// @return Returns a Delegate that will call _NotifyEvent(EventPtr) on this subscriber.
    EventSubscriberBinding::CallbackType GetDelegate();

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited

    /// @brief Method used to notify this Subscriber an event it cares about has occured.
    /// @param Args The specific information regarding the event that was fired.
    virtual void _NotifyEvent(EventPtr Args);
};//CatchQueuedSubscriber

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a convenience class used by some of the settings related work units.
/// @details
///////////////////////////////////////
class QueuedSettingsSubscriber : public CatchQueuedSubscriber
{
public:
    /// @brief A pointer to the manager that this events will eventually change the settings on.
    ManagerBase* TargetManager;
    /// @brief A pointer to the UIManager to give this access to the subscribed widgets.
    UI::UIManager* UITarget;

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    /// @brief Class constructor.
    /// @param UIMan A pointer to the UIManager used for Widget lookups if needed.
    /// @param Target The target manager this subscriber will alter.
    QueuedSettingsSubscriber(UI::UIManager* UIMan, ManagerBase* Target);
    /// @brief Class destructor.
    virtual ~QueuedSettingsSubscriber();
};//QueuedSettingsSubscriber

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a convenience class used to alter the pause state of the game.
/// @details
///////////////////////////////////////
class QueuedPauseSubscriber : public CatchQueuedSubscriber
{
public:
    /// @brief A pointer to the manager that this events will eventually change the settings on.
    CatchApp* CatchApplication;
    /// @brief A pointer to the UIManager to give this access to the subscribed widgets.
    UI::UIManager* UITarget;

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    /// @brief Class constructor.
    /// @param Target A pointer to the Catch root.
    /// @param UIMan A pointer to the UIManager used for Widget lookups if needed.
    QueuedPauseSubscriber(CatchApp* Target, UI::UIManager* UIMan);
    /// @brief Class destructor.
    virtual ~QueuedPauseSubscriber();
};//QueuedSettingsSubscriber

#endif
