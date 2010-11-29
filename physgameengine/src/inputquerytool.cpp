//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _inputquerytool_cpp
#define _inputquerytool_cpp

#include "inputquerytool.h"
#include "eventmanager.h"
#include "eventuserinput.h"
#include "world.h"

namespace phys
{
    InputQueryTool::InputQueryTool()
        : MouseXCache(0),
          MouseYCache(0)
    {
        this->GameWorld = World::GetWorldPointer();
    }

    InputQueryTool::~InputQueryTool()
    {
    }

    Whole InputQueryTool::GetMouseX()
    { return MouseXCache; }

    Whole InputQueryTool::GetMouseY()
    { return MouseYCache; }

    Vector2 InputQueryTool::GetMouseCoordinates()
    { return MouseCoordinates; }

    Vector2 InputQueryTool::GetMousePrevFrameOffset()
    { return MousePrevFrameOffset; }

    bool InputQueryTool::IsMouseButtonPushed(short unsigned int MouseButton)
    {
        if(MouseButton >= this->MouseButtonLimit)
            {this->GameWorld->LogAndThrow("Unsupported mouse button access through WorldQueryTool");}
        return MouseButtonCache.count(MouseButton);
    }

    bool InputQueryTool::IsKeyboardButtonPushed(MetaCode::InputCode KeyboardButton)
    {
        return KeyboardButtonCache.count(KeyboardButton);
    }

    MetaCode::ButtonState InputQueryTool::GetMouseButtonState(short unsigned int MouseButton)
    {
        std::map<short unsigned int,MetaCode>::iterator it = MouseButtonCache.find(MouseButton);
        if(it != MouseButtonCache.end())
        {
            int Code = (*it).second.GetMetaValue();
            switch (Code)
            {
                case -1:
                    return MetaCode::BUTTON_LIFTING;
                    break;
                case 1:
                    return MetaCode::BUTTON_PRESSING;
                    break;
                case 2:
                    return MetaCode::BUTTON_DOWN;
                    break;
                default:
                    break;
            }
        }
        return MetaCode::BUTTON_UP;
    }

    MetaCode::ButtonState InputQueryTool::GetKeyboardButtonState(MetaCode::InputCode KeyboardButton)
    {
        std::map<unsigned int,MetaCode>::iterator it = KeyboardButtonCache.find(KeyboardButton);
        if(it != KeyboardButtonCache.end())
        {
            int Code = (*it).second.GetMetaValue();
            switch (Code)
            {
                case -1:
                    return MetaCode::BUTTON_LIFTING;
                    break;
                case 1:
                    return MetaCode::BUTTON_PRESSING;
                    break;
                case 2:
                    return MetaCode::BUTTON_DOWN;
                    break;
                default:
                    break;
            }
        }
        return MetaCode::BUTTON_UP;
    }

    MetaCode::MouseWheelState InputQueryTool::GetMouseWheelState()
    {
        return WheelState;
    }

    void InputQueryTool::GatherEvents(bool ClearEventsFromEventMgr)
    {
        KeyboardButtonCache.clear();
        MouseButtonCache.clear();
        WheelState = MetaCode::MOUSEWHEEL_UNCHANGED;
        Vector2 PrevPos((Real)MouseXCache,(Real)MouseYCache);
        std::list<EventUserInput*>* UserInput = this->GameWorld->GetEventManager()->GetAllUserInputEvents();   // Get the updated list of events
        if( ClearEventsFromEventMgr )
            { this->GameWorld->GetEventManager()->RemoveAllSpecificEvents(EventBase::UserInput); }

        for(std::list<EventUserInput*>::iterator Iter = UserInput->begin(); Iter!=UserInput->end(); Iter++) //for each event
        {
            for(unsigned int c = 0; c<(*Iter)->GetMetaCodeCount(); c++) //For each metacode in the event
            {                                                           //Newer Items should take precedence of older ones, so only store the oldest ones
                if( (*Iter)->GetMetaCode(c).IsKeyboardButton() ) //is it a Key
                {
                    if(0 != (*Iter)->GetMetaCode(c).GetMetaValue()) //see MetaCode::ButtonState
                    {
                        this->KeyboardButtonCache[(*Iter)->GetMetaCode(c).GetCode()] = (*Iter)->GetMetaCode(c);
                    }
                    continue;
                }

                if ( MetaCode::MOUSEABSOLUTEHORIZONTAL == (*Iter)->GetMetaCode(c).GetCode() )
                {
                    this->MouseXCache = (*Iter)->GetMetaCode(c).GetMetaValue();
                    continue;
                }

                if ( MetaCode::MOUSEABSOLUTEVERTICAL == (*Iter)->GetMetaCode(c).GetCode() )
                {
                    this->MouseYCache = (*Iter)->GetMetaCode(c).GetMetaValue();
                    continue;
                }

                if ( MetaCode::MOUSEBUTTON == (*Iter)->GetMetaCode(c).GetCode() )
                {
                    if(0 != (*Iter)->GetMetaCode(c).GetMetaValue()) //see MetaCode::ButtonState
                    {
                        this->MouseButtonCache[(*Iter)->GetMetaCode(c).GetID()] = (*Iter)->GetMetaCode(c);
                    }
                    continue;
                }

                if ( MetaCode::MOUSEWHEELVERTICAL == (*Iter)->GetMetaCode(c).GetCode() )
                {
                    int code = (*Iter)->GetMetaCode(c).GetMetaValue();
                    if(-1 == code)
                    {
                        this->WheelState = MetaCode::MOUSEWHEEL_DOWN;
                    }
                    else if(1 == code)
                    {
                        this->WheelState = MetaCode::MOUSEWHEEL_UP;
                    }
                    continue;
                }
                /// @todo Add support for joysticks events to InputQueryTool
            }
        }
        MouseCoordinates = Vector2((Real)MouseXCache,(Real)MouseYCache);
        MousePrevFrameOffset = MouseCoordinates - PrevPos;

        if( ClearEventsFromEventMgr )//Erase everything if we were asked to.
        {
            for(std::list<EventUserInput*>::iterator Iter = UserInput->begin(); !UserInput->empty(); Iter = UserInput->begin())
            {
                delete(*Iter);
                UserInput->remove(*Iter);
            }
        }
    }
}

#endif
