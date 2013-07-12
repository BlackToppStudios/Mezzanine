//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _inputinputmanager_cpp
#define _inputinputmanager_cpp

#include "inputmanager.h"
#include "eventmanager.h"
#include "eventuserinput.h"
#include "entresol.h"
#include "Input/mouse.h"
#include "Input/keyboard.h"
#include "Input/controller.h"
#include "timer.h"

#include "SDL.h"

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class InputManagerInternalData
        /// @brief This is an class for the handling of internal input data.
        /// @details
        ///////////////////////////////////////
        class InputManagerInternalData
        {
            public:
                typedef std::pair<Input::Controller*,SDL_Joystick*> ControllerPair;
                typedef std::vector< ControllerPair >               ControllerContainer;
                typedef ControllerContainer::iterator               ControllerIterator;
                typedef ControllerContainer::const_iterator         ConstControllerIterator;
            protected:
            public:
                ControllerContainer Controllers;

                /// @brief Class constructor.
                InputManagerInternalData()
                    {  }
                /// @brief Class destructor.
                ~InputManagerInternalData()
                    { this->Controllers.clear(); }
        };//InputManangerInternalData

        typedef InputManagerInternalData::ControllerPair InternalControlPair;
        typedef InputManagerInternalData::ControllerIterator InternalControlIterator;

        ///////////////////////////////////////////////////////////////////////////////
        // DeviceUpdateWorkUnit Methods

        DeviceUpdateWorkUnit::DeviceUpdateWorkUnit(const DeviceUpdateWorkUnit& Other)
            {  }

        DeviceUpdateWorkUnit& DeviceUpdateWorkUnit::operator=(const DeviceUpdateWorkUnit& Other)
            { return *this; }

        DeviceUpdateWorkUnit::DeviceUpdateWorkUnit(InputManager* Target) :
            TargetManager(Target) {  }

        DeviceUpdateWorkUnit::~DeviceUpdateWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void DeviceUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            // Set up our containers for the update
            this->TargetManager->InputDeltas.clear();
            std::vector< Input::MetaCode > MouseCodes;
            std::vector< Input::MetaCode > KeyboardCodes;
            std::vector< std::vector< Input::MetaCode > > ControllerCodes;
            ControllerCodes.resize( this->TargetManager->GetNumControllers() );
            // And finally our container for our generated codes
            std::vector< Input::MetaCode > GeneratedCodes;

            // Get the user input events for processing
            std::list<EventUserInput*>* UserInput = EventManager::GetSingletonPtr()->GetAllUserInputEvents();
            EventManager::GetSingletonPtr()->RemoveAllSpecificEvents(EventBase::UserInput);

            // Process the aquired user input events
            while( !UserInput->empty() )
            {
                EventUserInput* CurrEvent = UserInput->front();
                for( Whole X = 0 ; X < CurrEvent->size() ; ++X )
                {
                    Input::MetaCode& CurrCode = CurrEvent->at(X);
                    this->TargetManager->InputDeltas.push_back(CurrCode);
                    if( Input::MOUSE_FIRST <= CurrCode.GetCode() && Input::MOUSE_LAST >= CurrCode.GetCode() ){
                        MouseCodes.push_back( CurrCode );
                    }else if( Input::KEY_FIRST <= CurrCode.GetCode() && Input::KEY_LAST >= CurrCode.GetCode() ){
                        KeyboardCodes.push_back( CurrCode );
                    }else if( Input::CONTROLLER_FIRST <= CurrCode.GetCode() && Input::CONTROLLER_LAST >= CurrCode.GetCode() ){
                        ControllerCodes[ CurrCode.GetDeviceIndex() ].push_back( CurrCode );
                    }
                }

                delete CurrEvent;
                UserInput->pop_front();
            }
            delete UserInput;
            UserInput = NULL;

            // Update all of our devices with the processed/saved data
            this->TargetManager->SystemMouse->_Update(MouseCodes,GeneratedCodes);
            this->TargetManager->SystemKeyboard->_Update(KeyboardCodes,GeneratedCodes);
            for( Whole X = 0 ; X < this->TargetManager->GetNumControllers() ; ++X )
            {
                this->TargetManager->IMID->Controllers.at(X).first->_Update( ControllerCodes.at(X) , GeneratedCodes );
            }
            // Do sub-system wide sequence checks if we've done anything
            if( !this->TargetManager->InputDeltas.empty() )
                this->TargetManager->Sequences.Update(this->TargetManager->InputDeltas,GeneratedCodes);
            // Update our delta's if there is anything to update
            if( !GeneratedCodes.empty() )
                this->TargetManager->InputDeltas.insert(this->TargetManager->InputDeltas.end(),GeneratedCodes.begin(),GeneratedCodes.end());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // InputManager Methods

        template<> InputManager* Singleton<InputManager>::SingletonPtr = NULL;

        InputManager::InputManager() :
            IMID(NULL),
            SystemMouse(NULL),
            SystemKeyboard(NULL),

            DeviceUpdateWork(NULL),
            ThreadResources(NULL)
        {
            UInt32 InitSDLSystems = SDL_WasInit(0);
            if( (SDL_INIT_JOYSTICK & InitSDLSystems) == 0 )
            {
                if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Joystick input, SDL Error: ") + SDL_GetError()); }
            }
            if( !(SDL_INIT_GAMECONTROLLER | InitSDLSystems) )
            {
                if( SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Game Controller input, SDL Error: ") + SDL_GetError()); }
            }

            this->IMID = new InputManagerInternalData();
            this->SystemMouse = new Mouse();
            this->SystemKeyboard = new Keyboard();
            this->DetectControllers();

            this->DeviceUpdateWork = new DeviceUpdateWorkUnit(this);
        }

        InputManager::InputManager(XML::Node& XMLNode) :
            IMID(NULL),
            SystemMouse(NULL),
            SystemKeyboard(NULL),

            DeviceUpdateWork(NULL),
            ThreadResources(NULL)
        {
            UInt32 InitSDLSystems = SDL_WasInit(0);
            if( (SDL_INIT_JOYSTICK & InitSDLSystems) == 0 )
            {
                if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Joystick input, SDL Error: ") + SDL_GetError()); }
            }
            if( !(SDL_INIT_GAMECONTROLLER | InitSDLSystems) )
            {
                if( SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Game Controller input, SDL Error: ") + SDL_GetError()); }
            }

            this->IMID = new InputManagerInternalData();
            this->SystemMouse = new Mouse();
            this->SystemKeyboard = new Keyboard();
            this->DetectControllers();

            this->DeviceUpdateWork = new DeviceUpdateWorkUnit(this);
        }

        InputManager::~InputManager()
        {
            this->Deinitialize();

            delete DeviceUpdateWork;

            delete SystemMouse;
            delete SystemKeyboard;
            this->ReleaseAllControllers();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // InputDevice Management

        Mouse* InputManager::GetSystemMouse() const
        {
            return this->SystemMouse;
        }

        Keyboard* InputManager::GetSystemKeyboard() const
        {
            return this->SystemKeyboard;
        }

        Controller* InputManager::GetController(const UInt16 Index) const
        {
            return this->IMID->Controllers.at(Index).first;
        }

        UInt16 InputManager::GetNumControllers() const
        {
            return this->IMID->Controllers.size();
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
                this->IMID->Controllers.push_back( InternalControlPair(NewController,InternalControl) );
            }
            return Count;
        }

        void InputManager::ReleaseAllControllers()
        {
            if( this->IMID->Controllers.empty() )
                return;

            for( InternalControlIterator ContIt = this->IMID->Controllers.begin() ; ContIt != this->IMID->Controllers.end() ; ++ContIt )
            {
                delete (*ContIt).first;
                SDL_JoystickClose( (SDL_Joystick*)(*ContIt).second );
            }
            this->IMID->Controllers.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Sequenced Input Management

        void InputManager::AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID)
            { this->Sequences.AddInputSequence(Codes,SequenceID); }

        bool InputManager::InputSequenceExists(const MetaCodeContainer& Codes)
            { return this->Sequences.InputSequenceExists(Codes); }

        Int32 InputManager::GetIDofInputSequence(const MetaCodeContainer& Codes)
            { return this->Sequences.GetIDofInputSequence(Codes); }

        void InputManager::RemoveInputSequence(const MetaCodeContainer& Codes)
            { this->Sequences.RemoveInputSequence(Codes); }

        void InputManager::RemoveAllInputSequences()
            { this->Sequences.RemoveAllInputSequences(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        const MetaCodeContainer& InputManager::GetInputDeltas() const
        {
            return InputDeltas;
        }

        void InputManager::Initialize()
        {
            if( !this->Initialized )
            {
                this->TheEntresol->GetScheduler().AddWorkUnitMain( this->DeviceUpdateWork );
                Mezzanine::EventManager* EventMan = EventManager::GetSingletonPtr();
                if( EventMan )
                    this->DeviceUpdateWork->AddDependency( EventMan->GetEventPumpWork() );

                this->Initialized = true;
            }
        }

        void InputManager::Deinitialize()
        {
            if( this->Initialized )
            {
                this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->DeviceUpdateWork );
                this->DeviceUpdateWork->ClearDependencies();

                this->Initialized = false;
            }
        }

        DeviceUpdateWorkUnit* InputManager::GetDeviceUpdateWork()
        {
            return this->DeviceUpdateWork;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

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
    }//Input
}//Mezzanine

#endif
