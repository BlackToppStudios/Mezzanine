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
        { CodeCache.insert(CodeCache.end(), MetaCode::INPUTEVENT_LAST, 0); }

    InputQueryTool::~InputQueryTool()
        {}

    int InputQueryTool::GetMouseX()
        { return CodeCache[MetaCode::MOUSEABSOLUTEHORIZONTAL] ;}
    int InputQueryTool::GetMouseY()
        { return CodeCache[MetaCode::MOUSEABSOLUTEVERTICAL] ;}
    int InputQueryTool::GetJoystickAxis(short unsigned int Axis)
        { return CodeCache[MetaCode::GetJoystickAxisCode(Axis)]; }


    Vector2 InputQueryTool::GetMouseCoordinates()
        { return Vector2(CodeCache[MetaCode::MOUSEABSOLUTEHORIZONTAL],CodeCache[MetaCode::MOUSEABSOLUTEVERTICAL]); }
    Vector2 InputQueryTool::GetMousePrevFrameOffset()
        { return Vector2(CodeCache[MetaCode::MOUSEHORIZONTAL],CodeCache[MetaCode::MOUSEVERTICAL]); }

    int InputQueryTool::GetRawMetaValue(MetaCode::InputCode AnyCode)
        { return CodeCache[AnyCode]; }

    bool InputQueryTool::IsMouseButtonPushed(short unsigned int MouseButton)
        { return MetaCode::BUTTON_UP < CodeCache[MetaCode::GetMouseButtonCode(MouseButton)]; }
    bool InputQueryTool::IsJoystickButtonPushed(short unsigned int JoyStickButton)
        { return MetaCode::BUTTON_UP < CodeCache[MetaCode::GetJoystickButtonCode(JoyStickButton)]; }
    bool InputQueryTool::IsKeyboardButtonPushed(MetaCode::InputCode KeyboardButton)
        { return MetaCode::BUTTON_UP < CodeCache[KeyboardButton]; }

    MetaCode::ButtonState InputQueryTool::GetMouseButtonState(short unsigned int MouseButton)
        { return (MetaCode::ButtonState)CodeCache[MetaCode::GetMouseButtonCode(MouseButton)]; }
    MetaCode::ButtonState InputQueryTool::GetKeyboardButtonState(MetaCode::InputCode KeyboardButton)
        { return (MetaCode::ButtonState)CodeCache[KeyboardButton]; }

    MetaCode::DirectionalMotionState InputQueryTool::GetMouseWheelState()
        { return (MetaCode::DirectionalMotionState)CodeCache[MetaCode::MOUSEWHEELVERTICAL]; }

    void InputQueryTool::GatherEvents(bool ClearEventsFromEventMgr)
    {
        CodeCache[MetaCode::MOUSEHORIZONTAL] = 0;           //We need to 0 out all
        CodeCache[MetaCode::MOUSEVERTICAL] = 0;

        std::list<EventUserInput*>* UserInput = World::GetWorldPointer()->GetEventManager()->GetAllUserInputEvents();   // Get the updated list of events

        if( ClearEventsFromEventMgr )
            { World::GetWorldPointer()->GetEventManager()->RemoveAllSpecificEvents(EventBase::UserInput); }

        MetaCode::InputCode ThisCode;
        for(std::list<EventUserInput*>::iterator Iter = UserInput->begin(); Iter!=UserInput->end(); Iter++) //for each event
        {
            for(unsigned int c = 0; c<(*Iter)->GetMetaCodeCount(); c++) //For each metacode in the event
            {
                ThisCode = (*Iter)->GetMetaCode(c).GetCode() ;
                if( ThisCode==MetaCode::MOUSEVERTICAL || ThisCode==MetaCode::MOUSEHORIZONTAL )
                {
                    CodeCache[ThisCode] += (*Iter)->GetMetaCode(c).GetMetaValue();
                }else{
                    CodeCache[ThisCode] = (*Iter)->GetMetaCode(c).GetMetaValue();
                }
            }
        }

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
