#ifndef _queuedsubscriber_cpp
#define _queuedsubscriber_cpp

#include "queuedsubscriber.h"
#include "catchapp.h"

///////////////////////////////////////////////////////////////////////////////
// QueuedSubscriber Methods

CatchQueuedSubscriber::CatchQueuedSubscriber() :
    Updating(false)
    {  }

CatchQueuedSubscriber::~CatchQueuedSubscriber()
    { this->ClearEvents(); }

///////////////////////////////////////////////////////////////////////////////
// Convenience and Utility

void CatchQueuedSubscriber::StartUpdate()
    { this->Updating = true; }

void CatchQueuedSubscriber::EndUpdate()
    { this->Updating = false; }

CatchQueuedSubscriber::ConstEventIterator CatchQueuedSubscriber::GetFirstEvent() const
    { return this->Events.begin(); }

CatchQueuedSubscriber::ConstEventIterator CatchQueuedSubscriber::GetEndEvent() const
    { return this->Events.end(); }

void CatchQueuedSubscriber::ClearEvents()
    { this->Events.clear(); }

CatchQueuedSubscriber::SubscriberType CatchQueuedSubscriber::GetDelegate()
{
    return SubscriberType(this,[this](UI::WidgetEventPtr Args){ this->_NotifyEvent(Args); });
}

///////////////////////////////////////////////////////////////////////////////
// Inherited

void CatchQueuedSubscriber::_NotifyEvent(EventPtr Args)
{
    if( !this->Updating ) {
        this->Events.push_back(Args);
    }
}

///////////////////////////////////////////////////////////////////////////////
// QueuedSettingsSubscriber Methods

QueuedSettingsSubscriber::QueuedSettingsSubscriber(UI::UIManager* UIMan, ManagerBase* Target) :
    TargetManager(Target),
    UITarget(UIMan)
    {  }

QueuedSettingsSubscriber::~QueuedSettingsSubscriber()
    {  }

///////////////////////////////////////////////////////////////////////////////
// QueuedSettingsSubscriber Methods

QueuedPauseSubscriber::QueuedPauseSubscriber(CatchApp* Target, UI::UIManager* UIMan) :
    CatchApplication(Target),
    UITarget(UIMan)
    {  }

QueuedPauseSubscriber::~QueuedPauseSubscriber()
    {  }

#endif
