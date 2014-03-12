#ifndef _queuedsubscriber_cpp
#define _queuedsubscriber_cpp

#include "queuedsubscriber.h"
#include "catchapp.h"

///////////////////////////////////////////////////////////////////////////////
// QueuedSubscriber Methods

QueuedSubscriber::QueuedSubscriber() :
    Updating(false)
    {  }

QueuedSubscriber::~QueuedSubscriber()
    { this->ClearEvents(); }

///////////////////////////////////////////////////////////////////////////////
// Convenience and Utility

void QueuedSubscriber::StartUpdate()
    { this->Updating = true; }

void QueuedSubscriber::EndUpdate()
    { this->Updating = false; }

QueuedSubscriber::ConstEventIterator QueuedSubscriber::GetFirstEvent() const
    { return this->Events.begin(); }

QueuedSubscriber::ConstEventIterator QueuedSubscriber::GetEndEvent() const
    { return this->Events.end(); }

void QueuedSubscriber::ClearEvents()
    { this->Events.clear(); }

///////////////////////////////////////////////////////////////////////////////
// Inherited

void QueuedSubscriber::_NotifyEvent(EventArgumentsPtr Args)
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
