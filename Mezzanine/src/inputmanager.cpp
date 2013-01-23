//© Copyright 2010 - 2012 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _inputmanager_cpp
#define _inputmanager_cpp

#include "inputmanager.h"
#include "eventmanager.h"
#include "eventuserinput.h"
#include "Input/mouse.h"
#include "Input/keyboard.h"
#include "Input/controller.h"

#include "SDL.h"

namespace Mezzanine
{
    template<> InputManager* Singleton<InputManager>::SingletonPtr = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // InputManager Methods

    InputManager::InputManager()
    {
        if( (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != SDL_WasInit(0) )
        {
            if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) == -1) //http://wiki.libsdl.org/moin.cgi/SDL_Init?highlight=%28\bCategoryAPI\b%29|%28SDLFunctionTemplate%29 // for more flags
            {
                MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for User input, SDL Error: ")+SDL_GetError());
            }
        }

        SystemMouse = new Input::Mouse();
        SystemKeyboard = new Input::Keyboard();
        DetectControllers();
        this->Priority = 5;
    }

    InputManager::InputManager(XML::Node& XMLNode)
    {
        if( (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != SDL_WasInit(0) )
        {
            if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) == -1) //http://wiki.libsdl.org/moin.cgi/SDL_Init?highlight=%28\bCategoryAPI\b%29|%28SDLFunctionTemplate%29 // for more flags
            {
                MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for User input, SDL Error: ")+SDL_GetError());
            }
        }

        SystemMouse = new Input::Mouse();
        SystemKeyboard = new Input::Keyboard();
        DetectControllers();
        this->Priority = 5;
    }

    InputManager::~InputManager()
    {
        delete SystemMouse;
        delete SystemKeyboard;
        ReleaseAllControllers();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // InputDevice Management

    Input::Mouse* InputManager::GetSystemMouse() const
    {
        return SystemMouse;
    }

    Input::Keyboard* InputManager::GetSystemKeyboard() const
    {
        return SystemKeyboard;
    }

    Input::Controller* InputManager::GetController(const UInt16 Index) const
    {
        return Controllers.at(Index);
    }

    UInt16 InputManager::GetNumControllers() const
    {
        return Controllers.size();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // InputDevice Detection

    UInt16 InputManager::DetectControllers()
    {
        UInt16 Count;
        for( Count = 0 ; Count < SDL_NumJoysticks() ; ++Count )
        {
            SDL_Joystick* InternalControl = SDL_JoystickOpen(Count);
            Input::Controller* NewController = new Input::Controller( InternalControl, Count );
            this->Controllers.push_back( NewController );
            this->InternalControllers.push_back( InternalControl );
        }
        return Count;
    }

    void InputManager::ReleaseAllControllers()
    {
        if( Controllers.empty() )
            return;

        for( ControllerIterator ContIt = Controllers.begin() ; ContIt != Controllers.end() ; ++ContIt )
        {
            delete (*ContIt);
        }
        for( std::vector<void*>::iterator ContIt = InternalControllers.begin() ; ContIt != InternalControllers.end() ; ++ContIt )
        {
            SDL_JoystickClose( (SDL_Joystick*)(*ContIt) );
        }
        this->Controllers.clear();
        this->InternalControllers.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //Inherited from ManagerBase

    void InputManager::Initialize()
        { Initialized = true; }

    void InputManager::DoMainLoopItems()
    {
        std::vector< MetaCode > MouseCodes;
        std::vector< MetaCode > KeyboardCodes;
        std::vector< std::vector< MetaCode > > ControllerCodes;
        ControllerCodes.resize( GetNumControllers() );

        std::list<EventUserInput*>* UserInput = EventManager::GetSingletonPtr()->GetAllUserInputEvents();
        EventManager::GetSingletonPtr()->RemoveAllSpecificEvents(EventBase::UserInput);

        for(std::list<EventUserInput*>::iterator Iter = UserInput->begin(); !UserInput->empty(); Iter = UserInput->begin())
        {
            EventUserInput* CurrEvent = (*Iter);
            for( Whole X = 0 ; X < CurrEvent->size() ; ++X )
            {
                MetaCode& CurrCode = CurrEvent->at(X);
                if( Input::MOUSE_FIRST <= CurrCode.GetCode() && Input::MOUSE_LAST >= CurrCode.GetCode() ){
                    MouseCodes.push_back( CurrCode );
                }else if( Input::KEY_FIRST <= CurrCode.GetCode() && Input::KEY_LAST >= CurrCode.GetCode() ){
                    KeyboardCodes.push_back( CurrCode );
                }else if( Input::CONTROLLER_FIRST <= CurrCode.GetCode() && Input::CONTROLLER_LAST >= CurrCode.GetCode() ){
                    ControllerCodes[ CurrCode.GetDeviceIndex() ].push_back( CurrCode );
                }
            }

            delete CurrEvent;
            UserInput->remove(*Iter);
        }

        SystemMouse->_Update(MouseCodes);
        SystemKeyboard->_Update(KeyboardCodes);
        for( Whole X = 0 ; X < GetNumControllers() ; ++X )
        {
            Controllers.at(X)->_Update( ControllerCodes.at(X) );
        }
    }

    ManagerBase::ManagerType InputManager::GetInterfaceType() const
        { return ManagerBase::InputManager; }

    String InputManager::GetImplementationTypeName() const
        { return "DefaultInputManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultInputManagerFactory Methods

    DefaultInputManagerFactory::DefaultInputManagerFactory()
    {
    }

    DefaultInputManagerFactory::~DefaultInputManagerFactory()
    {
    }

    String DefaultInputManagerFactory::GetManagerTypeName() const
    {
        return "DefaultInputManager";
    }

    ManagerBase* DefaultInputManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(InputManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return InputManager::GetSingletonPtr();
        }else return new InputManager();
    }

    ManagerBase* DefaultInputManagerFactory::CreateManager(XML::Node& XMLNode)
    {
        if(InputManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return InputManager::GetSingletonPtr();
        }else return new InputManager(XMLNode);
    }

    void DefaultInputManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

#endif
